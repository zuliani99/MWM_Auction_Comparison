#include "../include/BipartiteGraph.h"
#include "../include/MaximumWeightedMatching.h"
#include "../include/Auction.h"
#include <iostream>
#include <stdexcept>

#define VERBOSE false

void check_empty_file()
{
	std::ifstream file;
	file.open("../data/results.csv");
	if (file) {
		std::ofstream ofs;
		ofs.open("../data/results.csv", std::ofstream::out | std::ofstream::trunc);
		ofs.close();
	}
}

int main(int argc, const char* argv[])
{

	int min, max;
	std::fstream stream;

	check_empty_file();

	stream.open("../data/results.csv", std::ios::out | std::ios::app);
	stream << "Edge per part,Execution Time MWM,Seconds MWM,Total Cost MWM,Execution Time AU,Seconds AU,Total Cost AU,Iterations AU,Winner Execution Time,Winner Total Cost\n";

	std::cout << "-------- MAXIMUM WEIGHTED MATCHING - AUCTION ALGORITHM BECHMARK --------\n\n";
	std::cout << "Please specify the starting number of vertices per part: ";
	std::cin >> min;
	std::cout << "Please specify the ending number of vertices per part: ";
	std::cin >> max;

	if (min > max)
		throw std::invalid_argument("The starting number must be greater or equal to the ending number");

	for (int n = min; n <= max; ++n)
	{
		std::cout << "\n\n\nGeneration of a Bipartite Graph with " << n << " vertices per part: ";

		Duration elapsed_mwm, elapsed_au;
		Weight total_cost_mwm, total_cost_au;
		int n_iteration_au = 0;

		Graph graph = generateData(n, true);
		if (boost::num_vertices(graph) != 2 * n /* || boost::num_edges(graph) != n * n*/)
			throw std::invalid_argument("Number of vertices or edges not correct");
		std::cout << "done\n\n";


		if (VERBOSE) {
			printGraph(graph);
			std::cout << "\n";
		}


		//MAXIMUM WEIGHTED MATCHING
		std::cout << "Execution of Maximum Weighted Matching...";
		total_cost_mwm = perform_mwm(graph, elapsed_mwm);
		std::cout << std::fixed
			<< "It took: " << fmt{ elapsed_mwm } << ", with total cost: " << (total_cost_mwm)
			<< "\n\n";


		//AUCTION ALGOROTHM
		std::cout << "Execution of Auction Algorithm...";
		total_cost_au = perform_au(graph, elapsed_au, n_iteration_au, VERBOSE);
		std::cout << std::fixed
			<< "It took: " << fmt{ elapsed_au } << ", with total cost: " << (total_cost_au)
			<< " and " << n_iteration_au << " iterations" << "\n\n";


		stream << n << "," << fmt{ elapsed_mwm } << "," << (elapsed_mwm / 1.0s) << "," << (total_cost_mwm) << "," <<
			fmt{ elapsed_au } << "," << (elapsed_au / 1.0s) << "," << (total_cost_au) << "," << n_iteration_au << "," <<
			((elapsed_mwm / 1.0s) == (elapsed_au / 1.0s) ? "None" : (elapsed_mwm / 1.0s) < (elapsed_au / 1.0s) ? "MWM" : "AU") << "," <<
			((total_cost_mwm == total_cost_au) ? "None" : (total_cost_mwm > total_cost_au) ? "MWM" : "AU") << "\n";
		
	}

	std::cout << "\n";

	return 0;
}
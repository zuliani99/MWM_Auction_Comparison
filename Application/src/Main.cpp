
#include "../include/BipartiteGraph.h"
#include "../include/MaximumWeightedMatching.h"
#include "../include/Auction.h"
#include <stdexcept>


inline void check_empty_file()
{
	std::ifstream file;
	file.open("../data/results.csv");
	if (file) {
		std::ofstream ofs;
		ofs.open("../data/results.csv", std::ofstream::out | std::ofstream::trunc);
		ofs.close();
	}
}

void get_input(int& min, int& max, int& verbose, int& fully_connected)
{
	std::cout << "-------- MAXIMUM WEIGHTED MATCHING - AUCTION ALGORITHM BECHMARK --------\n\n";
	std::cout << "Do you want to active VERBOSE mode? (1/0) ";
	std::cin >> verbose;
	std::cout << "On what type of bipartite gaph would you like to work with, compelte (1) or non complete (0): ";
	std::cin >> fully_connected;
	std::cout << "Please specify the starting number of vertices per part: ";
	std::cin >> min;
	std::cout << "Please specify the ending number of vertices per part: ";
	std::cin >> max;

	if (min > max || (verbose != 0 && verbose != 1) || (fully_connected != 0 && fully_connected != 1))
		throw std::invalid_argument("Please insert correct input");

}

int main(int argc, const char* argv[])
{

	int min, max, verbose, fully_connected;
	std::fstream stream;

	check_empty_file();

	get_input(min, max, verbose, fully_connected);

	stream.open("../data/results.csv", std::ios::out | std::ios::app);
	stream << "Edge per part,Execution Time MWM,Seconds MWM,Total Cost MWM,"
		"Execution Time AU,Seconds nAU,Total Cost AU,Iterations nAU,"
		"Execution Time ncAU,Seconds ncAU,Total Cost ncAU,Iterations ncAU,"
		"Auction Winner, Winner Execution Time, Winner Total Cost\n"; //Auction Winner

	
	for (int n = min; n <= max; ++n)
	{
		std::cout << "\n\n\nGeneration of a Bipartite Graph with " << n << " vertices per part: ";

		Weight total_cost_mwm = 0;
		Duration elapsed_mwm;

		std::vector<Duration> elapsed_au(2); //elapsed_mwm, elapsed_au;
		std::vector<Weight> total_cost_au(2); // total_cost_mwm, total_cost_au;
		Weight best_total_cost_au = 0;
		Duration best_elapsed_au;
		int best_n_iter_au;
		std::string au_winner;
		std::vector<int> n_iteration_au(2);

		Graph graph = generateData(n, fully_connected);

		if (boost::num_vertices(graph) != 2 * static_cast<unsigned long long>(n)) //|| boost::num_edges(graph) != static_cast<unsigned long long>(n) * n)
			throw std::invalid_argument("Number of vertices or edges not correct");
		std::cout << "done\n\n";


		if (verbose) {
			printGraph(graph);
			std::cout << "\n";
		}


		//MAXIMUM WEIGHTED MATCHING
		std::cout << "Execution of Maximum Weighted Matching...";
		total_cost_mwm = perform_mwm(graph, elapsed_mwm);
		std::cout << std::fixed
			<< "It took: " << fmt{ elapsed_mwm } << ", with total cost: " << (total_cost_mwm) // / 10'000.0
			<< "\n\n";


		//AUCTION ALGOROTHM
		std::cout << "Execution of Auction Algorithm...";
		auto ret = perform_au(graph,total_cost_au, elapsed_au, n_iteration_au, best_elapsed_au, best_n_iter_au, verbose);
		au_winner = ret.first;
		best_total_cost_au = ret.second;
		//total_cost_au = perform_au(graph, elapsed_au, n_iteration_au, verbose);
		std::cout << std::fixed
			<< "It took: " << fmt{ best_elapsed_au } << ", with total cost: " << (best_total_cost_au) // / 10'000.0
			<< " and " << best_n_iter_au << " iterations" << "       " << au_winner << "\n\n";


		//Saving data in .csv file
		stream << n << "," << fmt{ elapsed_mwm } << "," << (elapsed_mwm / 1.0s) << "," << (total_cost_mwm) << "," << // / 10'000.0
			fmt{ elapsed_au[0] } << "," << (elapsed_au[0] / 1.0s) << "," << (total_cost_au[0]) << "," << n_iteration_au[0] << "," <<
			fmt{ elapsed_au[1] } << "," << (elapsed_au[1] / 1.0s) << "," << (total_cost_au[1]) << "," << n_iteration_au[1] << "," <<
			fmt{ best_elapsed_au } << "," << (best_elapsed_au / 1.0s) << "," << (best_total_cost_au) << "," << best_n_iter_au << "," << au_winner << "," << // / 10'000.0
			((elapsed_mwm / 1.0s) == (best_elapsed_au / 1.0s) ? "None" : (elapsed_mwm / 1.0s) < (best_elapsed_au / 1.0s) ? "MWM" : "AU") << "," <<
			((total_cost_mwm == best_total_cost_au) ? "None" : (total_cost_mwm > best_total_cost_au) ? "MWM" : "AU") << "\n";
		
	}

	std::cout << "\n";

	return 0;
}
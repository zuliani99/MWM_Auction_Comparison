
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
	std::cout << "On what type of bipartite graph would you like to work with, compelte (1) or non complete (0): ";
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
		"Execution Time nAU,Seconds nAU,Total Cost nAU,Iterations nAU,"
		"Execution Time eAU,Seconds eAU,Total Cost eAU,Iterations eAU,"
		"Auction Winner, Winner Execution Time, Winner Total Cost\n";

	
	for (int n = min; n <= max; ++n)
	{
		std::cout << "\n\n\nGeneration of a Bipartite Graph with " << n << " vertices per part: ";

		Weight total_cost_mwm = 0;
		Duration elapsed_mwm;


		std::map<std::string, RunAuction> auction_results;
		auction_results.insert(std::pair<std::string, RunAuction>("naive_auction", RunAuction(n)));
		auction_results.insert(std::pair<std::string, RunAuction>("e_scaling", RunAuction(n)));
		auction_results.insert(std::pair<std::string, RunAuction>("none", RunAuction(0)));

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
		std::cout << "Execution of Auction Algorithms...\n";
		std::string best_auction = perform_au(graph, auction_results, verbose);
		//total_cost_au = perform_au(graph, elapsed_au, n_iteration_au, verbose);
		std::cout << std::fixed
			<< "The best strategy: " << best_auction << " took: " << fmt{ auction_results.at(best_auction).elapsed} << ", with total cost: " << (auction_results.at(best_auction).cost) // / 10'000.0
			<< " and " << auction_results.at(best_auction).iterations << " iterations\n\n";


		//Saving data in .csv file
		stream << n << "," << fmt{ elapsed_mwm } << "," << (elapsed_mwm / 1.0s) << "," << (total_cost_mwm) << "," << // / 10'000.0
			fmt{ auction_results.at("naive_auction").elapsed} << "," << (auction_results.at("naive_auction").elapsed / 1.0s) << "," << (auction_results.at("naive_auction").cost) << "," << auction_results.at("naive_auction").iterations << "," <<
			fmt{ auction_results.at("e_scaling").elapsed } << "," << (auction_results.at("e_scaling").elapsed / 1.0s) << "," << (auction_results.at("e_scaling").cost) << "," << auction_results.at("e_scaling").iterations << "," << best_auction << "," <<
			//fmt{ auction_results.at(best_auction).elapsed } << "," << (auction_results.at(best_auction).elapsed / 1.0s) << "," << (auction_results.at(best_auction).cost) << "," << auction_results.at(best_auction).iterations << "," << best_auction << "," << // / 10'000.0
			((elapsed_mwm / 1.0s) == (auction_results.at(best_auction).elapsed / 1.0s) ? "None" : (elapsed_mwm / 1.0s) < (auction_results.at(best_auction).elapsed / 1.0s) ? "MWM" : "AU") << "," <<
			((total_cost_mwm == auction_results.at(best_auction).cost) ? "None" : (total_cost_mwm > auction_results.at(best_auction).cost) ? "MWM" : "AU") << "\n";
		
	}

	std::cout << "\n";

	return 0;
}
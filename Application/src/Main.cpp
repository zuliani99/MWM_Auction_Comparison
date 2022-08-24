
#include "../include/BipartiteGraph.h"
#include "../include/MaximumWeightedMatching.h"
#include "../include/Auction.h"
#include "../include/Utils.hpp"


int main(int argc, const char* argv[])
{

	int min, max, verbose, fully_connected;
	std::fstream stream;
	std::vector<int> epsilon(7);
	std::iota(epsilon.begin(), epsilon.end(), 4);

	check_empty_file();

	get_input(min, max, verbose, fully_connected);

	stream.open("../data/results.csv", std::ios::out | std::ios::app);
	stream << "Edge per part,Execution Time MWM,Seconds MWM,Total Cost MWM,"
		"Execution Time nAU,Seconds nAU,Total Cost nAU,Iterations nAU,";
	for(int& ep : epsilon)
		stream << "Execution Time eAU_" << ep << ",Seconds eAU_" << ep << ",Total Cost eAU_" << ep << ",Iterations eAU_" << ep << ",";
	stream << "Auction Winner, Winner Execution Time, Winner Total Cost\n";

	
	for (int n = min; n <= max; ++n)
	{
		std::cout << "\n\n\nGeneration of a Bipartite Graph with " << n << " vertices per part: ";

		Weight total_cost_mwm = 0;
		Duration elapsed_mwm;
		


		std::map<std::string, RunAuction> auction_results;
		auction_results.insert(std::pair<std::string, RunAuction>("naive_auction", RunAuction(n)));
		for (int& ep : epsilon)
			auction_results.insert(std::pair<std::string, RunAuction>("e_scaling_" + std::to_string(ep), RunAuction(n, ep)));
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
		stream << n << "," << fmt{ elapsed_mwm } << "," << (elapsed_mwm / 1.0s) << "," << (total_cost_mwm) << ",";
		for (auto& ar : auction_results)
			if (ar.first != "none")
				stream << fmt{ ar.second.elapsed } << "," << (ar.second.elapsed / 1.0s) << "," << (ar.second.cost) << "," << ar.second.iterations << ",";
		stream << ((elapsed_mwm / 1.0s) == (auction_results.at(best_auction).elapsed / 1.0s) ? "None" : (elapsed_mwm / 1.0s) < (auction_results.at(best_auction).elapsed / 1.0s) ? "MWM" : "AU") << "," <<
			((total_cost_mwm == auction_results.at(best_auction).cost) ? "None" : (total_cost_mwm > auction_results.at(best_auction).cost) ? "MWM" : "AU") << "\n";
		
	}

	std::cout << "\n";

	return 0;
}
#include "../include/BipartiteGraph.h"
#include "../include/MaximumWeightedMatching.h"
#include "../include/Auction.h"
#include "../include/Utils.hpp"


int main(int argc, const char* argv[])
{

	int min, max, verbose, fully_connected;
	std::fstream stream;
	std::vector<int> scaling_factors(7);
	std::iota(scaling_factors.begin(), scaling_factors.end(), 4);
	std::reverse(scaling_factors.begin(), scaling_factors.end());

	check_empty_file(); // Empty the file if exists

	get_input(min, max, verbose, fully_connected); // Obtaining the user inputs

	// Create the .csv file and writhe the first row
	stream.open("../data/results.csv", std::ios::out | std::ios::app);
	stream << "Edges_per_part,Execution_Time_MWM,Seconds_MWM,Total_Cost_MWM,";
	for(int& sf : scaling_factors)
		stream << "Execution_Time_eAU_" << sf << ",Seconds_eAU_" << sf << ",Total_Cost_eAU_" << sf << ",Iterations_eAU_" << sf << ",";
	stream << "Execution_Time_orAU,Seconds_orAU,Total_Cost_orAU,Iterations_orAU,Auction_Winner,Winner_Execution_Time,Winner_Total_Cost\n";

	
	for (int n = min; n <= max; ++n)
	{
		std::cout << "\n\n\nGeneration of a Bipartite Graph with " << n << " vertices per part: ";

		Weight total_cost_mwm = 0;
		Duration elapsed_mwm;
		
		// Map for temporal storing of the Auction Algorithms run
		std::map<std::string, RunAuction> auction_results;
		auction_results.insert(std::pair<std::string, RunAuction>("auction_original", RunAuction(n)));


		for (int& sf : scaling_factors)
		{
			std::ostringstream string_sf;
			string_sf.precision(3);
			string_sf << std::fixed << (round((1.0 / sf) * 1000.0) / 1000.0);
			auction_results.insert(std::pair<std::string, RunAuction>("auction_e_scaling_" + string_sf.str(), RunAuction(n, sf)));
		}
			
		auction_results.insert(std::pair<std::string, RunAuction>("none", RunAuction(0)));

		Graph graph = generateData(n, fully_connected); // Generation of a random bipartite graph

		// Check the if the generated graph is suitable for an Assignemnt Problem
		if ((fully_connected && (boost::num_vertices(graph) != 2 * static_cast<unsigned long long>(n) || boost::num_edges(graph) != static_cast<unsigned long long>(n) * n)) 
			|| (!fully_connected && boost::num_vertices(graph) != 2 * static_cast<unsigned long long>(n)))
			throw std::invalid_argument("Number of vertices or edges not correct");
		std::cout << "done\n\n";


		if (verbose)
		{
			printGraph(graph);
			std::cout << "\n";
		}


		// MAXIMUM WEIGHTED MATCHING
		std::cout << "Execution of Maximum Weighted Matching...";
		total_cost_mwm = perform_mwm(graph, elapsed_mwm);
		std::cout << std::fixed
			<< "It took: " << fmt{ elapsed_mwm } << ", with total cost: " << (total_cost_mwm)
			<< "\n\n";


		// AUCTION ALGOROTHM
		std::cout << "Execution of Auction Algorithms...\n";
		std::string best_auction = perform_au(graph, auction_results, verbose);
		std::cout << std::fixed
			<< "The best strategy: " << best_auction << " took: " << fmt{ auction_results.at(best_auction).elapsed} << ", with total cost: " << (auction_results.at(best_auction).cost)
			<< " and " << auction_results.at(best_auction).iterations << " iterations\n\n";


		// Saving data in .csv file
		stream << n << "," << fmt{ elapsed_mwm } << "," << (elapsed_mwm / 1.0s) << "," << (total_cost_mwm) << ",";
		for (auto& ar : auction_results)
			if (ar.first != "none")
				stream << fmt{ ar.second.elapsed } << "," << (ar.second.elapsed / 1.0s) << "," << (ar.second.cost) << "," << ar.second.iterations << ",";
		stream << best_auction << "," << ((elapsed_mwm / 1.0s) == (auction_results.at(best_auction).elapsed / 1.0s) ? "None" : (elapsed_mwm / 1.0s) < (auction_results.at(best_auction).elapsed / 1.0s) ? "MWM" : "AU") << "," <<
			((total_cost_mwm == auction_results.at(best_auction).cost) ? "None" : (total_cost_mwm > auction_results.at(best_auction).cost) ? "MWM" : "AU") << "\n";
		
	}

	std::cout << "\n";

	return EXIT_SUCCESS;
}
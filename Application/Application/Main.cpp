#include "BipartiteGraph.h"
#include "MaximumWeightedMatching.h"
#include "Auction.h"

constexpr auto MIN = 2;
constexpr auto MAX = 8;

int main(int argc, const char* argv[]) {
	srand((unsigned int)time(0));
	//int n_bidders_items = 3;
	for (int n_bidders_items = MIN; n_bidders_items <= MAX; ++n_bidders_items) {
		std::cout << "\n\n\nGeneration of a Bipartite Graph with " << n_bidders_items << " per part\n";

		//long long time_execution_mwm;
		long long time_execution_auction;
		//float total_cost_mwm = 0.;
		float total_cost_auction = 0.;

		Graph graph = generateData(n_bidders_items);
		printGraph(graph);
        
		//std::cout << "The graph is bipartite? ";
		//boost::is_bipartite(graph) ? std::cout << "Yes\n" : std::cout << "No\n";


		//MAXIMUM WEIGHTED MATCHING
		//std::cout << "\nExecution of Maximum Weighted Matching\n";
		//maximum_weight_matching(graph, time_execution_mwm, total_cost_mwm);
		//std::cout << "Execution time of Maximum Weight Matching: " << float(time_execution_mwm) / 1000 << " milliseconds, with total cost: " << total_cost_mwm << "\n\n";

		//AUCTION ALGOROTHM
		std::cout << "\nExecution of Auction Algorithm\n";
		auction_algorithm(graph, time_execution_auction, total_cost_auction);
		std::cout << "Execution time of Maximum Weight Matching: " << float(time_execution_auction) / 1000 << " milliseconds, with total cost: " << total_cost_auction << "\n\n";
		printGraph(graph);
	}

	return 0;
}


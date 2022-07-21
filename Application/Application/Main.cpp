#include "BipartiteGraph.h"
#include "MaximumWeightedMatching.h"
#include "Auction.h"
#include <boost/lexical_cast.hpp>
#include <string>

constexpr auto MAX = 10;
constexpr auto MIN = 5;

int main(int argc, const char* argv[]) {
	srand((unsigned int)time(0));
	const int n_bidders_items = 4; //rand() % (MAX - MIN + 1) + MIN;
	long long time_execution_mwm;
	long long time_execution_auction;
	float total_cost_mwm = 0.;
	float total_cost_auction = 0.;

	Graph graph = generateData(n_bidders_items);
	//printGraph(graph);

	//MAXIMUM WEIGHTED MATCHING
	std::cout << "\nExecution of Maximum Weighted Matching\n";
	maximum_weight_matching(graph, time_execution_mwm, total_cost_mwm);
	std::cout << "Execution time of Maximum Weight Matching: " << float(time_execution_mwm) / 1000 << " milliseconds, with total cost: " << total_cost_mwm << "\n\n";


	//AUCTION ALGOROTHM
	std::cout << "Execution of Auction Algorithm\n";
	auction_algorithm(graph, time_execution_auction, total_cost_auction);
	std::cout << "Execution time of Maximum Weight Matching: " << float(time_execution_mwm) / 1000 << " milliseconds, with total cost: " << total_cost_mwm << "\n\n";
	//printGraph(graph);

	return 0;
}
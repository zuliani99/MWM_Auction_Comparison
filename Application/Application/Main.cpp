#include "BG.h"
#include "MWM.h"
#include "Auction.h"

constexpr auto MAX = 10;
constexpr auto MIN = 3;


int main(int argc, const char* argv[]){
	srand((unsigned int)time(0));
	const int vertices_per_part = rand() % (MAX - 3 + MIN) + MIN;

	long long time_execution_mwm;
	long long time_execution_auction;
	float total_cost_mwm = 0.;
	float total_cost_auction = 0.;
	std::vector<std::vector<float>> *cost_matrix = new std::vector<std::vector<float>>(vertices_per_part * 2, std::vector<float> (vertices_per_part * 2, 0));
	
	UndirectedGraph graph = return_graph(cost_matrix, vertices_per_part);
	//maximum_weight_matching(return_graph(cost_matrix, vertices_per_part));
	
	std::cout << "Edges weigth matrix:\n";
	for (int i = 0; i < vertices_per_part* 2; ++i) {
		for (int j = 0; j < vertices_per_part * 2; ++j) {
			std::cout << (*cost_matrix)[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "\n\n";

	std::cout << "Execution of Maximum Weight Matching\n";
	maximum_weight_matching(graph, &time_execution_mwm, &total_cost_mwm);
	std::cout << "Execution time of Maximum Weight Matching: " << float(time_execution_mwm) / 1000 << ", with total cost: " << total_cost_mwm << std::endl;


	/*std::cout << "Execution of Auction Algorithm\n";
	std::vector<int> bidder_item = auction_algorithm(cost_matrix, &vertices_per_part, &time_execution_auction);
	for (int bidder = 0; bidder < vertices_per_part; ++bidder) {
		std::cout << "Bidder: " << bidder << " has item: " << bidder_item[bidder] << std::endl;
		total_cost_auction += (*cost_matrix)[bidder][bidder_item[bidder]];
	}
	std::cout << "Execution time of Auction Algorithm: " << float(time_execution_auction) / 1000 << ", with total cost: " << total_cost_auction << std::endl;
	*/

	return 0;
}
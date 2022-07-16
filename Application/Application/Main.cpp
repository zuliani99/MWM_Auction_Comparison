#include "BG.h"
#include "MWM.h"
#include "Auction.h"

constexpr auto MAX = 10;
constexpr auto MIN = 3;


int main(int argc, const char* argv[]){
	srand((unsigned int)time(0));

	const int vertices_per_part = rand() % (MAX - 3 + MIN) + MIN;

	std::vector<std::vector<float>> *cost_matrix = new std::vector<std::vector<float>>(vertices_per_part * 2, std::vector<float> (vertices_per_part * 2, 0));
	
	//UndirectedGraph graph = return_graph(cost_matrix, vertices_per_part);
	maximum_weight_matching(return_graph(cost_matrix, vertices_per_part));
	
	std::cout << "Edges weigth matrix: " << std::endl;
	for (int i = 0; i < vertices_per_part* 2; ++i) {
		for (int j = 0; j < vertices_per_part * 2; ++j) {
			std::cout << (*cost_matrix)[i][j] << "\t";
		}
		std::cout << std::endl;
	}



	//std::pair<long long, std::vector<int>> res = auction_algorithm(cost_matrix, &vertices_per_part);

	//std::cout << res.first << std::endl;

	/*for (int i = 0; i < vertices_per_part; ++i) {
		std::cout << "Bidder: " << i << " has item: " << res.second[i];
	}*/

	return 0;
}
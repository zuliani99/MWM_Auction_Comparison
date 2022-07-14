#include "BG_example.h"
#include "MWM_example.h"
#include "Auction.h"

int main(int argc, const char* argv[]){
	srand((unsigned int)time(0));

	const int vertices_per_part = rand() % (10 - 3 + 1) + 3;
	//std::cout << vertices_per_part;
	double* cost_matrix = (double*)malloc(vertices_per_part * vertices_per_part * sizeof(double));
	int* bidder2item = (int*)malloc(vertices_per_part * sizeof(int));

	UndirectedGraph graph = return_graph(cost_matrix, vertices_per_part);

	//maximum_weight_matching(graph);

	/*
	auction_algorithm(cost_matrix, vertices_per_part, bidder2item);

	for (int i = 0; i < vertices_per_part; ++i) {
		std::cout << "cdcdcdc";
		std::cout << "Bidder: " << i << " has item: " << bidder2item[i];
	}
	*/

	return 0;
}
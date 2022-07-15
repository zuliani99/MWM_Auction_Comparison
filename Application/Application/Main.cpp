#include "BG_example.h"
#include "MWM_example.h"
#include "Auction.h"

constexpr int MAX = 10;
constexpr int MIN = 3;



int main(int argc, const char* argv[]){

	srand((unsigned int)time(0));

	const int vertices_per_part = rand() % (MAX - 3 + MIN) + MIN;

	double* cost_matrix = (double*)malloc(vertices_per_part * vertices_per_part * sizeof(double));
	int* bidder2item = (int*)malloc(vertices_per_part * sizeof(int));
	
	
	UndirectedGraph graph = return_graph(cost_matrix, vertices_per_part);


	auction_algorithm(cost_matrix, vertices_per_part, bidder_item);

	for (int i = 0; i < vertices_per_part; ++i) {
		std::cout << "Bidder: " << i << " has item: " << bidder2item[i];
	}

	return 0;
}
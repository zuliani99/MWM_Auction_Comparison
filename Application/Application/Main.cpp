#include "BG_example.h"
#include "MWM_example.h"
#include "Auction.h"

int MAX = 10;
int MIN = 3;



int main(int argc, const char* argv[]){
	srand((unsigned int)time(0));

	const int vertices_per_part = rand() % (MAX - 3 + MIN) + MIN;
	//std::vector<std::vector<double>> cost_matrix (vertices_per_part, std::vector<double>(vertices_per_part));
	
	double *cost_matrix = (double*)malloc(vertices_per_part * vertices_per_part * sizeof(double));
	
	UndirectedGraph graph = return_graph(cost_matrix, vertices_per_part);


	long long elapsed = auction_algorithm(cost_matrix, vertices_per_part);

	/*for (int i = 0; i < vertices_per_part; ++i) {
		std::cout << "Bidder: " << i << " has item: " << bidder_item[i];
	}*/

	return 0;
}
#include "BipartiteGraph.h"
#include "MaximumWeightedMatching.h"
#include "Auction.h"

int main(int argc, const char* argv[]) {

	int min, max;

	std::cout << "-------- MAXIMUM WEIGHTED MATCHING - AUCTION ALGORITHM BECHMARCK --------\n\n";
	std::cout << "Please specify the starting number of verticies per part: ";
	std::cin >> min;
	std::cout << "Please specify the ending number of verticies per part: ";
	std::cin >> max;

	for (int n = min; n <= max; ++n) {
		std::cout << "\n\n\nGeneration of a Bipartite Graph with " << n << " per part\n";

		duration elapsed_mwm, elapsed_au;
		Weight total_cost_mwm, total_cost_au;
		bool fully_connected = true;

		Graph graph = generateData(n, fully_connected);
		assert(boost::num_vertices(graph) == 2 * n);
		assert(boost::num_edges(graph) == n * n);
		printGraph(graph);
        
		//std::cout << "The graph is bipartite? ";
		//boost::is_bipartite(graph) ? std::cout << "Yes\n" : std::cout << "No\n";

		//MAXIMUM WEIGHTED MATCHING
		total_cost_mwm = perform_mwm(graph, elapsed_mwm);
		std::cout << "Execution time of Maximum Weight Matching: " << std::fixed
			<< fmt{ elapsed_mwm } << ", with total cost: " << (total_cost_mwm / 10'000.0)
			<< "\n\n";
		

		//AUCTION ALGOROTHM
		total_cost_au = perform_au(graph, elapsed_au);
		std::cout << "Execution time of Auction Algorithm: " << std::fixed
			<< fmt{ elapsed_au } << ", with total cost: " << (total_cost_au / 10'000.0)
			<< "\n\n";

		std::cout << "Same solution? ";
		(total_cost_mwm == total_cost_au) ? std::cout << "Yes" : std::cout << "No";
	}

	return 0;
}


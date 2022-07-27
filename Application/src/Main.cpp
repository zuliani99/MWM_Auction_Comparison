#include "../include/BipartiteGraph.h"
#include "../include/MaximumWeightedMatching.h"
#include "../include/Auction.h"
#include "../include/AuctionArray.h"
#define VERBOSE false

void check_empty_file() {
	std::ifstream file;
	file.open("../data/results.csv");
	if (file) {
		std::ofstream ofs;
		ofs.open("../data/results.csv", std::ofstream::out | std::ofstream::trunc);
		ofs.close();
	}
}

int main(int argc, const char* argv[]) {

	int min, max;
	std::fstream stream;

	check_empty_file();

	stream.open("../data/results.csv", std::ios::out | std::ios::app);
	stream << "Edge per part,Execution Time MWM,Seconds MWM,Total Cost MWM,Execution Time AU,Seconds AU,Total Cost AU,Execution Time AU-A,Seconds AU-A,Total Cost AU-A,Winner Execution Time,Winner Total Cost\n";

	std::cout << "-------- MAXIMUM WEIGHTED MATCHING - AUCTION ALGORITHM BECHMARCK --------\n\n";
	std::cout << "Please specify the starting number of verticies per part: ";
	std::cin >> min;
	std::cout << "Please specify the ending number of verticies per part: ";
	std::cin >> max;

	assert(("The starting number must be greater or equal to the ending number", min <= max));

	for (int n = min; n <= max; ++n) {
		std::cout << "\n\n\nGeneration of a Bipartite Graph with " << n << " per part: ";

		duration elapsed_mwm, elapsed_au, elapsed_aua;
		Weight total_cost_mwm, total_cost_au, total_cost_aua;

		//Graph graph = generateData(n);
		auto [cost_matrix, graph] = generateData(n);
		assert(("Number of verticies not correct", boost::num_vertices(graph) == 2 * n));
		assert(("Number of edges not correct", boost::num_edges(graph) == n * n));
		std::cout << "done\n";
		
		if (VERBOSE) printGraph(graph);

		//MAXIMUM WEIGHTED MATCHING
		std::cout << "Execution time of Maximum Weight Matching";
		total_cost_mwm = perform_mwm(graph, elapsed_mwm);
		std::cout << std::fixed
			<< fmt{elapsed_mwm} << ", with total cost: " << (total_cost_mwm / 10'000.0)
			<< "\n\n";
		

		//AUCTION  ALGOROTHM
		std::cout << "Execution time of Auction Algorithm";
		total_cost_au = perform_au(graph, elapsed_au);
		std::cout << std::fixed
			<< fmt{elapsed_au} << ", with total cost: " << (total_cost_au / 10'000.0)
			<< "\n\n";

		//AUCTION ARRAY ALGOROTHM
		std::cout << "Execution time of Auction Algorithm";
		//total_cost_au = perform_au(graph, elapsed_au);
		total_cost_aua = perform_au_array(cost_matrix, n, elapsed_aua);
		std::cout << std::fixed
			<< fmt{elapsed_aua} << ", with total cost: " << (total_cost_aua / 10'000.0)
			<< "\n\n";
		
		//std::cout << "Same solution? ";
		//(total_cost_mwm == total_cost_au) ? std::cout << "Yes" : std::cout << "No";
		
		stream << n << "," << fmt{elapsed_mwm} << "," << (elapsed_mwm / 1.0s) << "," << (total_cost_mwm / 10'000.0) << "," <<
			fmt{elapsed_au} << "," << (elapsed_au / 1.0s) << "," << (total_cost_au / 10'000.0) << "," <<
			fmt{elapsed_aua} << "," << (elapsed_aua / 1.0s) << "," << (total_cost_aua / 10'000.0) << "," <<
			((elapsed_mwm / 1.0s) == (elapsed_aua / 1.0s) ? "None" : (elapsed_mwm / 1.0s) < (elapsed_aua / 1.0s) ? "MWM" : "AU") << "," <<
			((total_cost_mwm == total_cost_au) ? "None" : (total_cost_mwm > total_cost_au) ? "MWM" : "AU") << "\n";
		
	}

	std::cout << "\n";

	return 0;
}

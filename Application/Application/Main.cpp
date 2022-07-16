#include "BG.h"
#include "MWM.h"
#include "Auction.h"
#include "TextTable.h"
#include <boost/lexical_cast.hpp>
#include <string>

constexpr auto MAX = 9;
constexpr auto MIN = 3;

int main(int argc, const char* argv[]){
	srand((unsigned int)time(0));
	const int vertices_per_part = rand() % (MAX - MIN + 1) + MIN;

	long long time_execution_mwm;
	long long time_execution_auction;
	float total_cost_mwm = 0.;
	float total_cost_auction = 0.;
	std::vector<std::vector<float>> *cost_matrix = new std::vector<std::vector<float>>(vertices_per_part * 2, std::vector<float> (vertices_per_part * 2, 0));
	
	UndirectedGraph graph = return_graph(cost_matrix, vertices_per_part);

	TextTable t('-', '|', '+');
	t.add("Bidder / Item");
	for(int i = 0; i < vertices_per_part * 2; ++i) t.add(boost::lexical_cast<std::string>(i));
	t.endOfRow();
	
	std::cout << "Edges weigth matrix:\n";
	for (int i = 0; i < vertices_per_part* 2; ++i) {
		t.add(std::to_string(i));
		for (int j = 0; j < vertices_per_part * 2; ++j) {
			//std::cout << boost::lexical_cast<std::string>((*cost_matrix)[i][j]) << "\t";
			t.add(boost::lexical_cast<std::string>((*cost_matrix)[i][j]));
		}
		//std::cout << std::endl;
		t.endOfRow();
	}
	t.setAlignment(vertices_per_part, TextTable::Alignment::RIGHT);
	std::cout << t << "\n\n";

	std::cout << "Execution of Maximum Weight Matching\n";
	maximum_weight_matching(graph, &time_execution_mwm, &total_cost_mwm);
	std::cout << "Execution time of Maximum Weight Matching: " << float(time_execution_mwm) / 1000 << ", with total cost: " << total_cost_mwm << "\n\n";

	/*std::cout << "Execution of Auction Algorithm\n";
	auction_algorithm(cost_matrix, &vertices_per_part, &time_execution_auction, &total_cost_auction);
	std::cout << "Execution time of Auction Algorithm: " << float(time_execution_auction) / 1000 << ", with total cost: " << total_cost_auction << "\n\n";
	*/

	return 0;
}
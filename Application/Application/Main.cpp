#include "BG_example.h"
#include "MWM_example.h"
#include "Auction.h"

int main(int argc, const char* argv[]){
	srand((unsigned int)time(0));

	UndirectedGraph graph = return_graph();
	maximum_weight_matching(graph);
	return 0;
}
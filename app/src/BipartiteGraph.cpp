#include "../include/BipartiteGraph.h"


// Function to generate a random bipartite graph, complete or incomplete, depending on the user input
Graph generateData(int N, const int& fully_connected)
{
	Graph g;

	Distribution<Weight>dist(1, 10000);
    Distribution<int>dist_edge(0, 1);
	std::map<int, int> map_first_conenction;

	std::vector<int> free_items;
	for(int i = 0; i < N; ++i)
		free_items.push_back(i);

	if (fully_connected) // Random complete bipartite graph
	{
		for (int bidder = 0; bidder < N; ++bidder)
			for (int item = 0; item < N; ++item)
				boost::add_edge(bidder, N + item, dist(prng), g);
	}
	else // Random incomplete bipartite graph
	{
		for (int bidder = 0; bidder < N; ++bidder)
		{
			Distribution<int> random_first_connection(0, static_cast<int>(free_items.size()) - 1);
			int pos = random_first_connection(prng);
			boost::add_edge(bidder, N + free_items[pos], dist(prng), g);
			map_first_conenction.insert(std::make_pair(bidder, free_items[pos]));
			free_items.erase(std::find(free_items.begin(), free_items.end(), free_items[pos]));

			for (int item = 0; item < N; ++item)
				if (map_first_conenction[bidder] != item)
					if (dist_edge(prng)) boost::add_edge(bidder, N + item, dist(prng), g);
		}
	}

	return g;
}


// Fucntion to print the graph proprieties
void printGraph(Graph& g)
{
    boost::dynamic_properties dp;
    dp.property("node_id", get(boost::vertex_index, g));
    dp.property("weight", get(boost::edge_weight, g));
    boost::write_graphviz_dp(std::cout, g, dp);
}
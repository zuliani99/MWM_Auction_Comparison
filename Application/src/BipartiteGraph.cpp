#include "../include/BipartiteGraph.h"

Graph generateData(int N)
{
    Graph g;

	Distribution<Weight>dist(1 * 10'000, 100 * 10'000); // 1 - 100
    Distribution<int>dist_edge(0, 1);

	for (int bidder = 0; bidder < N; ++bidder)
		for (int item = 0; item < N; ++item)
			add_edge(bidder, N + item, dist(prng), g);

    return g;
}

void printGraph(Graph& g)
{
    boost::dynamic_properties dp;
    dp.property("node_id", get(boost::vertex_index, g));
    dp.property("weight", get(boost::edge_weight, g));
    boost::write_graphviz_dp(std::cout, g, dp);
}
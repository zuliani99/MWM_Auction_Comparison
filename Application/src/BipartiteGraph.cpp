#include "../include/BipartiteGraph.h"
#include <map>

Graph generateData(int N)
{
    Graph g;

    Distribution<Weight>dist(10'000, 400'000); // 1 - 40
    Distribution<int>dist_edge(0, 1);
	std::map<int, int> map_first_conenction;

	std::vector<int> free_items(N);
	for(int i = 0; i < N; ++i)
		free_items.push_back(i);

    //for (int i = 0; i < N; ++i) boost::add_vertex(Bidder{ i }, g);
    //for (int i = 0; i < N; ++i) boost::add_vertex(Item{ i }, g);

    //GraphProp& gp = g[boost::graph_bundle];
    //gp.bidder2item.assign(N, -1);
    //gp.item2bidder.assign(N, -1);


    /*for (int bidder = 0; bidder < N; ++bidder)
	{
		int count = 0;
		for (int item = 0; item < N; ++item)
            //add_edge(bidder, N + item, dist(prng), g);
		{
			if(dist_edge(prng)) 
			{
				count +=1;
				add_edge(bidder, N + item, dist(prng), g);
			}
			else add_edge(bidder, N + item, 0, g);
		}
	}*/

	for (int bidder = 0; bidder < N; ++bidder)
	{
		Distribution<int> random_first_connection(0, free_items.size());
		int pos = random_first_connection(prng);
		add_edge(bidder, N + free_items[pos], dist(prng), g);
		map_first_conenction.insert(std::make_pair(bidder, free_items[pos]));
		free_items.erase(std::find(free_items.begin(), free_items.end(), free_items[pos]));

		for (int item = 0; item < N; ++item)
			if(map_first_conenction[bidder] != item && dist_edge(prng))
				add_edge(bidder, N + item, dist(prng), g);
	}
		
            

    //std::cout << add_edge(0, N, dist(prng), g).second << "\n";

    return g;
}

void printGraph(Graph& g)
{
    boost::dynamic_properties dp;
    dp.property("node_id", get(boost::vertex_index, g));
    //dp.property("label", get(boost::vertex_bundle, g));
    dp.property("weight", get(boost::edge_weight, g));
    boost::write_graphviz_dp(std::cout, g, dp);
}
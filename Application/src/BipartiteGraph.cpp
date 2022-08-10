#include "../include/BipartiteGraph.h"

Graph generateData(int N)
{
    Graph g;

    Distribution<Weight>dist(10'000, 400'000); // 1 - 40
    //Distribution<int>dist_edge(0, 1);

    //for (int i = 0; i < N; ++i) boost::add_vertex(Bidder{ i }, g);
    //for (int i = 0; i < N; ++i) boost::add_vertex(Item{ i }, g);

    //GraphProp& gp = g[boost::graph_bundle];
    //gp.bidder2item.assign(N, -1);
    //gp.item2bidder.assign(N, -1);


    for (int bidder = 0; bidder < N; ++bidder)
        for (int item = 0; item < N; ++item)
            add_edge(bidder, N + item, dist(prng), g);
    /* {
        if(dist_edge(prng)) add_edge(bidder, N + item, dist(prng), g);
        else add_edge(bidder, N + item, 0, g);
    } */

    return g;
}

/*void printGraph(Graph& g)
{
    boost::dynamic_properties dp;
    dp.property("node_id", get(boost::vertex_index, g));
    dp.property("label", get(boost::vertex_bundle, g));
    dp.property("weight", get(boost::edge_weight, g));
    boost::write_graphviz_dp(std::cout, g, dp);
}*/
#include "BipartiteGraph.h"

std::default_random_engine generator;

Graph generateData(int N) {
    Graph g;

    std::uniform_real_distribution<float> distribution(0., 20.);

    for (int i = 0; i < N; ++i) boost::add_vertex(Bidder{ i }, g);
    for (int i = 0; i < N; ++i) boost::add_vertex(Item{ i }, g);

    GraphProp& gp = g[boost::graph_bundle];
    gp.bidder2item.assign(N, -1);
    gp.item2bidder.assign(N, -1);

    // Every left nodes has a connection to every right nodes
    for (int bidder = 0; bidder < N; ++bidder) {
        for (int item = 0; item < N; ++item) {
            float value = distribution(generator);

            //float value = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5 - 1)));
            boost::add_edge(bidder, N + item, value, g);
        }
    }

    return g;
}



void printGraph(Graph& g) {
    boost::dynamic_properties dp;
    dp.property("node_id", get(boost::vertex_index, g));
    dp.property("label", get(boost::vertex_bundle, g));
    dp.property("weight", get(boost::edge_weight, g));
    write_graphviz_dp(std::cout, g, dp);
}
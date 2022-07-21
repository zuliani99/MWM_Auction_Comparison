#include "BipartiteGraph.h"

std::default_random_engine generator;

Data generateData(int N) {
    Data data;
    auto& [cm, g] = data;

    data.cost_matrix.assign(N, Matrix::value_type(N, 0));
    std::uniform_real_distribution<float> distribution(0., 30.);

    for (int i = 0; i < N; ++i) boost::add_vertex(Bidder{ i }, g);
    for (int i = 0; i < N; ++i) boost::add_vertex(Item{ N + i }, g);

    GraphProp& gp = g[boost::graph_bundle];
    gp.bidder2item.assign(N, -1);
    gp.item2bidder.assign(N, -1);

    // Every left nodes has a connection to every right nodes
    for (int bidder = 0; bidder < N; ++bidder) {
        for (int item = 0; item < N; ++item) {
            float value = distribution(generator);
            data.cost_matrix[bidder][item] = value;
            boost::add_edge(bidder, N + item, value, g);
        }
    }

    return data;
}


void printGraph(Graph& g) {
    boost::dynamic_properties dp;
    dp.property("node_id", get(boost::vertex_index, g));
    dp.property("label", get(boost::vertex_bundle, g));
    dp.property("weight", get(boost::edge_weight, g));
    write_graphviz_dp(std::cout, g, dp);
}
#include "../include/BipartiteGraph.h"

Graph generateData(int N) {
    Graph g;
//Data generateData(int N) {
    //Data data;
    //auto& [cm, g] = data;

	//cm.assign(N, Matrix::value_type(N, 0));

    Distribution dist(10'000, 400'000);
    Distribution int_dist(0, 1);
    Distribution int_dist_addedge(0, N - 1);

    for (int i = 0; i < N; ++i) boost::add_vertex(Bidder{ i }, g);
    for (int i = 0; i < N; ++i) boost::add_vertex(Item{ i }, g);

    //printGraph(g);

    GraphProp& gp = g[boost::graph_bundle];
    gp.bidder2item.assign(N, -1);
    gp.item2bidder.assign(N, -1);


    // Every left nodes has a connection to every right nodes
    for (int bidder = 0; bidder < N; ++bidder) {
        for (int item = 0; item < N; ++item) {
            add_edge(bidder, N + item, dist(prng), g);
			/*auto value = dist(prng);
            cm[bidder][item] = value;
            add_edge(bidder, N + item, value, g);*/
        }
    }

    //printGraph(g);

    return g;
	//return data;
}


void printGraph(Graph& g) {
    boost::dynamic_properties dp;
    dp.property("node_id", get(boost::vertex_index, g));
    dp.property("label", get(boost::vertex_bundle, g));
    dp.property("weight", get(boost::edge_weight, g));
    boost::write_graphviz_dp(std::cout, g, dp);
}
#include "BipartiteGraph.h"

Data generateData(int N, bool fully_connected) {
    Data data;
    auto& [cm, g] = data;

    cm.assign(N, Matrix::value_type(N, 0));

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
            if (!fully_connected)
                if (!int_dist(prng)) continue;
            auto value = dist(prng);
            cm[bidder][item] = value;
            add_edge(bidder, N + item, value, g);
        }
    }

    //printGraph(g);

    if (!fully_connected) {
        while (!check_graph(g, N)) {
            sanitize_edge_bidder(g, N, dist, int_dist_addedge);
            sanitize_edge_item(g, N, dist, int_dist_addedge);
        }
    }

    return data;
}


bool check_graph(Graph& g, int N) {
    for (int i = 0; i < N; ++i) {
        int count_bidder = 0, count_item = 0;
        for (int j = 0; j < N; ++j) {
            if (boost::edge(i, j + N, g).second) count_bidder++;
            if (boost::edge(i + N, j, g).second) count_item++;
        }
        //std::cout << "check" << count_bidder << " " << count_item << "\n";
        if (count_bidder == 0 || count_item == 0) return false;
    }
    return true;
}

void sanitize_edge_bidder(Graph& g, int N, Distribution dist, Distribution int_dist_addedge) {
    for (int bidder = 0; bidder < N; ++bidder) {
        int count_bidder = 0;
        for (int item = 0; item < N; ++item) {
            if (boost::edge(bidder, item + N, g).second) count_bidder++;
        }
        //std::cout << "check" << count_bidder << "\n";
        if (count_bidder == 0) boost::add_edge(bidder, N + int_dist_addedge(prng), dist(prng), g);
    }
}


void sanitize_edge_item(Graph& g, int N, Distribution dist, Distribution int_dist_addedge) {
    for (int item = 0; item < N; ++item) {
        int count_item = 0;
        for (int bidder = 0; bidder < N; ++bidder) {
            if (boost::edge(item + N, bidder, g).second) count_item++;
        }
        //std::cout << "check" << count_item << "\n";
        if (count_item == 0) boost::add_edge(int_dist_addedge(prng), item + N, dist(prng), g);
    }
}


void printGraph(Graph& g) {
    boost::dynamic_properties dp;
    dp.property("node_id", get(boost::vertex_index, g));
    dp.property("label", get(boost::vertex_bundle, g));
    dp.property("weight", get(boost::edge_weight, g));
    boost::write_graphviz_dp(std::cout, g, dp);
}
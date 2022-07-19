#include "BG.h"

std::default_random_engine generator;

UndirectedGraph return_graph(std::vector<std::vector<float>> *cost_matrix, int n_bidders_items) {
    std::uniform_real_distribution<float> distribution(0., 20.);

    UndirectedGraph random_graph;

    // Every left nodes has a connection to every right nodes
    for (int i = 0; i < n_bidders_items; ++i)  // For left verticies
        for (int j = n_bidders_items; j < n_bidders_items * 2; ++j)
            if (i != j) {
                //float value = float(ceilf(distribution(generator) * 1000.0) / 1000.0); //round 3 decimals
                float value = float(distribution(generator));
                (*cost_matrix)[i][j % n_bidders_items] = value;
                boost::add_edge(i, j, value, random_graph);
            }
    //print_edge_graph(random_graph);
    return random_graph;
}

void print_edge_graph(UndirectedGraph graph) {
    EdgeWeightMap Map = get(boost::edge_weight_t(), graph);

    std::cout << "Edges weights of a random bipartite graph with " << num_vertices(graph) / 2 << " vertices per part\n";

    std::pair<edge_iterator, edge_iterator> edgePair;
    for (edgePair = edges(graph); edgePair.first != edgePair.second; ++edgePair.first)
        std::cout << *edgePair.first << " " << Map[*edgePair.first] << "\n";
    std::cout << "\n\n";
}
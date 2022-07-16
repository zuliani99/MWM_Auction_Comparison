#include "BG.h"

std::default_random_engine generator;

void return_graph(std::vector<std::vector<double>> *cost_matrix, int n_vertices_per_part) {
    std::uniform_real_distribution<double> distribution(0., 20.);

    UndirectedGraph random_graph(n_vertices_per_part * 2);

    // Every left nodes has a connection to every right nodes
    for (int i = 0; i < n_vertices_per_part; ++i)  // For left verticies
        for (int j = n_vertices_per_part; j < n_vertices_per_part * 2; ++j)
            if (i != j) {
                double value = ceilf(distribution(generator) * 1000.0) / 1000.0; //round 3 decimals
                (*cost_matrix)[i][j] = (*cost_matrix)[j][i] = value;
                add_edge(i, j, value, random_graph);
            }
    print_edge_graph(random_graph);
    //return random_graph;
}

void print_edge_graph(UndirectedGraph graph) {
    EdgeWeightMap Map = get(boost::edge_weight_t(), graph);

    std::cout << "Edges weights of a random bipartite graph with " << num_vertices(graph) / 2 << " vertices per part\n\n";

    std::pair<edge_iterator, edge_iterator> edgePair;
    for (edgePair = edges(graph); edgePair.first != edgePair.second; ++edgePair.first)
        std::cout << *edgePair.first << " " << Map[*edgePair.first] << "\n";
    std::cout << "\n\n";
}
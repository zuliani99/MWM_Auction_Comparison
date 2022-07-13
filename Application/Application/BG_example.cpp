#include "BG_example.h"


int random(int min, int max) {
    return rand() % (max - min + 1) + min;
}

UndirectedGraph return_graph() {
    int n_vertices_per_part = random(3, 6); // Generate the number, assign to variable.

    UndirectedGraph random_graph(n_vertices_per_part * 2);

    // Every left nodes has a connection to every right nodes
    for (int i = 0; i < n_vertices_per_part; ++i)  // For left verticies
        /*for (int j = 0; j < random(0, n_vertices_per_part); ++j) {
            int destination;
            do {
                destination = random(n_vertices_per_part, n_vertices_per_part * 2);
            } while (boost::edge(i, destination, random_graph).second);
            add_edge(i, destination, random(1, 20), random_graph);
        }*/
        for (int j = n_vertices_per_part; j < n_vertices_per_part * 2; ++j)
            if(i != j)
                add_edge(i, j, random(1, 20), random_graph);
    print_edge_graph(random_graph);
    return random_graph;
}

void print_edge_graph(UndirectedGraph graph) {
    EdgeWeightMap Map = get(boost::edge_weight_t(), graph);

    std::cout << "Edges weights of a random bipartite graph:" << std::endl;

    std::pair<edge_iterator, edge_iterator> edgePair;
    for (edgePair = edges(graph); edgePair.first != edgePair.second; ++edgePair.first)
        std::cout << *edgePair.first << " " << Map[*edgePair.first] << std::endl;
    std::cout << std::endl << std::endl;
}
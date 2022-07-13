#include <iostream>
#include <vector>
#include <string>
#include <random>
#include "BG_example.h"


int random(int min, int max) {
    srand(time(0));
    return rand() % (max - min + 1) + min;
}

random_bipartite_graph return_graph() {

    int n_vertices_per_part = random(5, 10); // Generate the number, assign to variable.
    random_bipartite_graph random_graph(n_vertices_per_part * 2);
    std::cout << n_vertices_per_part << std::endl;

    for (int i = 0; i < n_vertices_per_part; ++i) {
        int n_connections = random(0, n_vertices_per_part);
        std::cout << n_connections << std::endl;
        for (int j = 0; j < n_connections; ++j) {
            add_edge(i, j, EdgeProperty(random(0, 20)), random_graph);
        }
    }
    return random_graph;
}

void print_edge_graph(random_bipartite_graph graph) {
    std::pair<edge_iterator, edge_iterator> ei = edges(graph);

    std::cout << "Number of edges = " << num_edges(graph) << "\n";
    std::cout << "Edge list:\n";

    std::copy(ei.first, ei.second,
        std::ostream_iterator<boost::adjacency_list<>::edge_descriptor>{
        std::cout, "\n"});

    std::cout << std::endl;
}

void generate_random_graph() {
    print_edge_graph(return_graph());
}
#ifndef _BG_H
#define _BG_H

#include "Graph.h"
#include <random>

UndirectedGraph return_graph(std::vector<std::vector<float>> *cost_matrix, int n_vertices_per_part);
void print_edge_graph(UndirectedGraph graph);

#endif 
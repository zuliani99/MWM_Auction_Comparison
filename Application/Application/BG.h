#ifndef _BG_H
#define _BG_H

#include "Graph.h"
#include <random>

void return_graph(std::vector<std::vector<double>> *cost_matrix, int n_vertices_per_part);
void print_edge_graph(UndirectedGraph graph);

#endif 
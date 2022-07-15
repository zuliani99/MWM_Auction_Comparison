#ifndef _BG_example_H
#define _BG_example_H

#include "Graph.h"
#include <random>

UndirectedGraph return_graph(double *cost_matrix, int n_vertices_per_part);
void print_edge_graph(UndirectedGraph graph);

#endif 
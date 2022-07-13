#ifndef _BG_example_H
#define _BG_example_H

#include "Graph.h"
#include <random>

int random(int min, int max);
UndirectedGraph return_graph();
void print_edge_graph(UndirectedGraph graph);

extern const int n_vertices_per_part;
#endif 
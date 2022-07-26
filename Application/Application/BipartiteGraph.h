#ifndef _BG_H
#define _BG_H

#include "Graph.h"
#include <random>

using Distribution = std::uniform_int_distribution<int64_t>;

Data generateData(int N, bool fully_connected);
void printGraph(Graph& g);
bool check_graph(Graph& g, int N);
void sanitize_edge_bidder(Graph& g, int N, Distribution dist, Distribution int_dist_addedge);
void sanitize_edge_item(Graph& g, int N, Distribution dist, Distribution int_dist_addedge);


#endif 
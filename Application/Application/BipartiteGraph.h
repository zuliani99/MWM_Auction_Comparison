#ifndef _BG_H
#define _BG_H

#include "Graph.h"
#include <random>
Graph generateData(int N, bool fully_connected);
void printGraph(Graph& g);
bool check_graph(Graph& g, int N);
void sanitize_edge_bidder(Graph& g, int N, std::uniform_real_distribution<float> float_dist, std::uniform_int_distribution<int> int_dist_addedge);
void sanitize_edge_item(Graph& g, int N, std::uniform_real_distribution<float> float_dist, std::uniform_int_distribution<int> int_dist_addedge);


#endif 
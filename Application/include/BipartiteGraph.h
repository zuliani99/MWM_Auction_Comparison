#ifndef _BG_H
#define _BG_H

#include "Graph.hpp"
#include <random>

using Distribution = std::uniform_int_distribution<Weight>;

Graph generateData(int N);
void printGraph(Graph& g);

#endif 
#ifndef _BG_H
#define _BG_H

#include "Graph.h"
#include <random>

using Distribution = std::uniform_int_distribution<int64_t>;

Graph generateData(int N);
void printGraph(Graph& g);

#endif 
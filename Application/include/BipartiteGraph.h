#ifndef _BG_H
#define _BG_H

#include "Graph.hpp"
#include <random>
#include <map>

template<typename T>
using Distribution = std::uniform_int_distribution<T>;

Graph generateData(int N, const char* fully_connected);
void printGraph(Graph& g);

#endif 
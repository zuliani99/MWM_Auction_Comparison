#ifndef _Auction_H
#define _Auction_H

#include "Graph.hpp"

std::pair<std::string, Weight> perform_au(const Graph& graph, Duration& elapsed, int& n_iteration_au, const int& verbose);

#endif
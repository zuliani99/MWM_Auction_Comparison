#ifndef _Auction_H
#define _Auction_H

#include "Graph.hpp"
#include "RunAuction.h"
#include <tuple>


std::string perform_au(const Graph& graph, std::map<std::string, RunAuction>& auction_results, const int& verbose);
void run_auction(const Graph& graph, const int& verbose, const std::string& name, RunAuction& runauc);

#endif
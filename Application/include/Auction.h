#ifndef _Auction_H
#define _Auction_H

#include "Graph.hpp"

template <typename Auc>
struct RunAuction
{
    Auc auction;
    std::vector<int> assignments;
    Weight cost = 0;
    int iterations = 0;
    Duration elapsed;
};

//std::pair<std::string, Weight>
std::pair<std::string, Weight> perform_au(const Graph& graph, std::vector<Weight>& c, std::vector<Duration>& el, std::vector<int>& it, Duration& elapsed, int& n_iteration_au, const int& verbose);

#endif
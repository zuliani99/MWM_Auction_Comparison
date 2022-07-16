#ifndef _Auction_H
#define _Auction_H

#include "Graph.h"
#include "Items.h"
#include "Bidders.h"

std::pair<long long, std::vector<int>> auction_algorithm(double* cost_matrix, const int *n_vertices_per_part);

template<typename Class>
std::unordered_map<int, Class> retunr_n(int n) {
    std::unordered_map<int, Class> ret;
    for (int i = 0; i < n; ++i) {
        ret.insert({ i, Class() });
    }
    return ret;
}

#endif
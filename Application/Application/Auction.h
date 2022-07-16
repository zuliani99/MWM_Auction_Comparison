#ifndef _Auction_H
#define _Auction_H

#include "Graph.h"
#include "Items.h"
#include "Bidders.h"

std::vector<int> auction_algorithm(std::vector<std::vector<float>>* cost_matrix, const int* n_vertices_per_part, long long *time_execution);

template<typename Class>
std::unordered_map<int, Class> retunr_n(int n) {
    std::unordered_map<int, Class> ret;
    for (int i = 0; i < n; ++i) {
        ret.insert({ i, Class() });
    }
    return ret;
}

#endif
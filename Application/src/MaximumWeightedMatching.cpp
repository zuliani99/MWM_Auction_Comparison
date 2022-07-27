#include "../include/MaximumWeightedMatching.h"

Weight perform_mwm(Graph const& graph, duration& elapsed) {
    auto const N = num_vertices(graph);
    std::vector<V> mate(N);

    auto t_start = now();
    maximum_weighted_matching(graph, &mate[0]);
    elapsed = now() - t_start;

    Weight cost = matching_weight_sum(graph, &mate[0]);

    std::cout << "\nThe matching is: ";

    for (V v : boost::make_iterator_range(vertices(graph)))
        if (mate[v] != Graph::null_vertex() && v < mate[v])
            std::cout << "(" << v << "," << (mate[v] - (N / 2)) << ")";
    std::cout << "\n";

    return cost;
}
#include "../include/MaximumWeightedMatching.h"

Weight perform_mwm(const Graph& graph, Duration& elapsed)
{
    auto const N = num_vertices(graph);
    std::vector<V> mate(N);

    auto t_start = now();
    maximum_weighted_matching(graph, &mate[0]);
    elapsed = now() - t_start;    

    if (std::find(mate.begin(), mate.end(), Graph::null_vertex()) != mate.end())
    {
        std::cout << " Finished \nNo matching found\n";

        return static_cast<Weight>(-1);
    }
    else
    {
        /* (FROM vertex, TO vertex, Verex WEIGHT )*/
        std::cout << " Finished \nThe matching is: ";

        for (V v : boost::make_iterator_range(vertices(graph)))
            if (mate[v] != Graph::null_vertex() && v < mate[v])
                std::cout << "(" << v << "," << (mate[v] - (N / 2)) << "," << (boost::get(boost::edge_weight_t(), graph, (boost::edge(v, mate[v], graph)).first)) / 10'000.0 << ")";

        std::cout << "\n";

        return matching_weight_sum(graph, &mate[0]);
    }

    
}

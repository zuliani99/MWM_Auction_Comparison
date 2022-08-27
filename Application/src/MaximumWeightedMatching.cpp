#include "../include/MaximumWeightedMatching.h"


Weight perform_mwm(const Graph& graph, Duration& elapsed)
{
    auto const N = boost::num_vertices(graph);
    std::vector<V> mate(N);

    auto t_start = now();
    boost::maximum_weighted_matching(graph, &mate[0]); // Execution of the Maximum Weighted Matching
    elapsed = now() - t_start;    

    if (std::find(mate.begin(), mate.end(), Graph::null_vertex()) != mate.end())
    {
        std::cout << " Finished \nNo matching found\n";

        return static_cast<Weight>(-1);
    }
    else
    {
        // If the algorithm was able to find a solution

        std::cout << " Finished \nThe matching is: ";

        // Print the Bidder - Item matchs with the edge weight, in the following form: (FROM vertex, TO vertex, Verex WEIGHT)
        for (V v : boost::make_iterator_range(vertices(graph)))
            if (mate[v] != Graph::null_vertex() && v < mate[v])
                std::cout << "(" << v << "," << (mate[v] - (N / 2)) << "," << (boost::get(boost::edge_weight_t(), graph, (boost::edge(v, mate[v], graph)).first)) << ")";

        std::cout << "\n";

        return matching_weight_sum(graph, &mate[0]); // Return the weighted sum of the matching
    }

    
}

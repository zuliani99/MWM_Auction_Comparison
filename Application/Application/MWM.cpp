#include "MWM.h"

void maximum_weight_matching(UndirectedGraph graph, long long *time_execution, float *total_cost){
    vertex_iterator vi, vi_end;

    std::vector <boost::graph_traits<UndirectedGraph>::vertex_descriptor> mate(boost::num_vertices(graph));

    auto t_start = std::chrono::high_resolution_clock::now();
    boost::maximum_weighted_matching(graph, &mate[0]);
    *time_execution = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t_start).count();
    *total_cost = float(boost::matching_weight_sum(graph, &mate[0]));


    std::cout << "The matching is:" << std::endl;
    for (boost::tie(vi, vi_end) = vertices(graph); vi != vi_end; ++vi)
        if (mate[*vi] != boost::graph_traits< UndirectedGraph >::null_vertex()
            && *vi < mate[*vi])
            std::cout << "Bidder: " << *vi << " has item " << mate[*vi] % (boost::num_vertices(graph) / 2) << "" << std::endl;
    std::cout << std::endl;
}
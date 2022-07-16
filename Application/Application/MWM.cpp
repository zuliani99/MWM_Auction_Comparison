#include "MWM.h"

void maximum_weight_matching(UndirectedGraph graph){
    vertex_iterator vi, vi_end;

    std::vector <boost::graph_traits<UndirectedGraph>::vertex_descriptor> mate(boost::num_vertices(graph));

    boost::maximum_weighted_matching(graph, &mate[0]);

    std::cout << "Found a weighted matching:" << std::endl;
    std::cout << "Matching size is " << boost::matching_size(graph, &mate[0])
        << ", total weight is " << boost::matching_weight_sum(graph, &mate[0])
        << std::endl;
    std::cout << std::endl;

    std::cout << "The matching is:" << std::endl;
    for (boost::tie(vi, vi_end) = vertices(graph); vi != vi_end; ++vi)
        if (mate[*vi] != boost::graph_traits< UndirectedGraph >::null_vertex()
            && *vi < mate[*vi])
            std::cout << "{" << *vi << ", " << mate[*vi] << "}" << std::endl;
    std::cout << std::endl;
}
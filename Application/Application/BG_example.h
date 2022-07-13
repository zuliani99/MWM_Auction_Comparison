#ifndef _BG_example_H
#define _BG_example_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/maximum_weighted_matching.hpp>
#include <boost/graph/graph_utility.hpp>

using namespace boost;

using EdgeProperty = property <edge_weight_t, float, property <edge_index_t, int>>;
using random_bipartite_graph = adjacency_list <vecS, vecS, undirectedS, no_property, EdgeProperty>;
using edge_iterator = graph_traits<random_bipartite_graph>::edge_iterator;
using vertex_iterator = graph_traits<random_bipartite_graph>::vertex_iterator;

extern const int n_vertices_per_part;

int random(int min, int max);
random_bipartite_graph return_graph();
void generate_random_graph();

#endif 
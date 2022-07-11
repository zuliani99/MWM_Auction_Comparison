#include <boost/graph/adjacency_list.hpp>
#include <iostream>

typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, boost::no_property, EdgeWeightProperty > DirectedGraph;
typedef boost::graph_traits<DirectedGraph>::edge_iterator edge_iterator;

int main()
{
    DirectedGraph g;

    boost::add_edge(0, 1, 8, g);
    boost::add_edge(0, 3, 18, g);
    boost::add_edge(1, 2, 20, g);
    boost::add_edge(2, 3, 2, g);
    boost::add_edge(3, 1, 1, g);
    boost::add_edge(1, 3, 7, g);
    boost::add_edge(1, 4, 1, g);
    boost::add_edge(4, 5, 6, g);
    boost::add_edge(2, 5, 7, g);

    std::pair<edge_iterator, edge_iterator> ei = edges(g);

    std::cout << "Number of edges = " << num_edges(g) << "\n";
    std::cout << "Edge list:\n";

    std::copy(ei.first, ei.second,
        std::ostream_iterator<boost::adjacency_list<>::edge_descriptor>{
        std::cout, "\n"});

    std::cout << std::endl;

    return 0;
}
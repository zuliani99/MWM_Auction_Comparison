#ifndef _Graph_H
#define _Graph_H

#include <vector>
#include <chrono>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/maximum_weighted_matching.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/grid_graph.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_map/function_property_map.hpp>
#include <boost/property_map/transform_value_property_map.hpp>
#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/bipartite.hpp>


namespace Nodes {
    struct Bidder {
        int    id;
        int    best_item = -1;
        float val_first_best_item = -1.;
        float val_second_best_item = -1.;
    };

    struct Item {
        int    id;
        float cost = 0.;
        int    high_bidder = -1;
        float high_bid = -1.;
    };
    
    static inline std::ostream& operator<<(std::ostream& os, Bidder const& b) {
        return os << "BIDDER:" << b.id << "|best_item:" << b.best_item
            << "|best1:" << b.val_first_best_item
            << "|best2:" << b.val_second_best_item;
    }
    static inline std::ostream& operator<<(std::ostream& os, Item const& i) {
        return os << "ITEM:" << i.id << "|cost:" << i.cost
            << "|high_bidder:" << i.high_bidder
            << "|high_bid:" << i.high_bid;
    }

    using VertexProp = boost::variant<Bidder, Item>;

    static inline std::istream& operator>>(std::istream& is, VertexProp&) { return is; }
}

using Nodes::Bidder;
using Nodes::Item;
using Nodes::VertexProp;


struct GraphProp {
    std::vector<int> bidder2item;
    std::vector<int> item2bidder;
};

using EdgeProp = boost::property<boost::edge_weight_t, float, boost::property<boost::edge_index_t, float>>;
using Graph = boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, VertexProp, EdgeProp, GraphProp>;
using vertex_iterator = boost::graph_traits<Graph>::vertex_iterator;
using V = Graph::vertex_descriptor;
using E = Graph::edge_descriptor;
using VertexFilter = std::function<bool(V)>;
using EdgeFilter = std::function<bool(E)>;
using FMap = boost::filtered_graph<Graph, EdgeFilter, VertexFilter>;

#endif
#ifndef _Graph_H
#define _Graph_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/maximum_weighted_matching.hpp>
#include <boost/graph/properties.hpp>
#include <boost/variant.hpp>

#include <chrono>
using namespace std::chrono_literals;
static auto now = std::chrono::high_resolution_clock::now;
using Duration = std::chrono::high_resolution_clock::duration;

#include <random>
static std::mt19937 prng{ std::random_device{}() };


/*namespace Nodes {
    using Weight = int64_t;

    struct Bidder {
        int    id;
        int    best_item = -1;
        Weight val_first_best_item = -1;
        Weight val_second_best_item = -1;
    };

    struct Item {
        int   id;
        Weight cost = 0;
        int    high_bidder = -1;
        Weight high_bid = -1;
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

    static inline std::istream& operator>>(std::istream& is, VertexProp&) {
        return is;
    }
} // namespace Nodes

using Nodes::Weight;
using Nodes::Bidder;
using Nodes::Item;
using Nodes::VertexProp;

struct GraphProp {
    std::vector<int> bidder2item;
    std::vector<int> item2bidder;
};*/
using Weight = int;
using EdgeProp = boost::property<boost::edge_weight_t, Weight>;
//using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexProp, EdgeProp, GraphProp>;
using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, EdgeProp>;
//using AdjacencyIterator = boost::graph_traits<Graph>::adjacency_iterator;
//using vertex_iterator = Graph::vertex_iterator;
using V = Graph::vertex_descriptor;
//using E = Graph::edge_descriptor;

struct fmt {
    Duration const& _d;

    friend std::ostream& operator<<(std::ostream& os, fmt f) {
        if (f._d >= 1min) return os << (f._d / 1min) << "min " << (f._d % 1min) / 1s << "s";
        else if (f._d >= 1s) return os << (f._d / 1.0s) << "s";
        else if (f._d >= 1ms) return os << (f._d / 1.0ms) << "ms";
        else return os << (f._d / 1.0us) << "us";
    }
};

#endif
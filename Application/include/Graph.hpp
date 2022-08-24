#ifndef _Graph_H
#define _Graph_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/maximum_weighted_matching.hpp>
#include <random>
#include <chrono>


using namespace std::chrono_literals;
static auto now = std::chrono::high_resolution_clock::now;
using Duration = std::chrono::high_resolution_clock::duration;
static std::mt19937 prng{ std::random_device{}() };


using Weight = int;
using EdgeProp = boost::property<boost::edge_weight_t, Weight>;
using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, EdgeProp>;
using V = Graph::vertex_descriptor;


// Struct used to manage the output of both algorithms execution time 
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
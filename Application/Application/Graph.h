#ifndef _Graph_H
#define _Graph_H

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/maximum_weighted_matching.hpp>
#include <boost/graph/graph_utility.hpp>
//#include "Structures.h"
#include <vector>


using EdgeWeight = boost::property<boost::edge_weight_t, double>;
using UndirectedGraph = boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, boost::no_property, EdgeWeight>;
using edge_iterator = boost::graph_traits<UndirectedGraph>::edge_iterator;
using vertex_iterator = boost::graph_traits<UndirectedGraph>::vertex_iterator;
using EdgeWeightMap = boost::property_map<UndirectedGraph, boost::edge_weight_t>::type;


#endif
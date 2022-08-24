#ifndef _AA_H
#define _AA_H

#include <vector>
#include <limits>
#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>


template<typename T>
using AdjacencyIterator = boost::graph_traits<T>::adjacency_iterator;

template<typename T>
using vertex_idMap = boost::property_map<T,boost::vertex_index_t>::type;

template<typename T>
using edge_iterator = boost::graph_traits<T>::edge_iterator;


// Class used to store all the useful information to solve the Assignment Problem
template<typename Graph, typename Type>
class Auction
{
    private:
        struct Bidder {
            int best_item = -1;
            double val_first_best_item = -1;
            double val_second_best_item = -1;
        };

        struct Item {
            double cost = 0;
            int high_bidder = -1;
            double high_bid = -1;
        };

        int n_iteration_au = 0;
        int vertices = 0;
		int scaling_factor = 1;

        std::unordered_map<int, Bidder> unassigned_bidder; // Unordered map that will store all the unassigned bidders
        std::unordered_map<int, Bidder> assigned_bidder; // Unordered map that will store all the assigned bidders
        std::unordered_map<int, Item> item_map; // Unordered map that will store all the items
        
        bool is_assignment_problem(const Graph& graph);
        void auctionRound(const Graph& graph, const double& eps, const vertex_idMap<Graph>& V_Map, bool& err);
        
    public:
		void original_auction(const Graph& graph, std::vector<int>& ass);
        void e_scaling_auction(const Graph& graph, std::vector<int>& ass, const double& scaling_factor);
        Type getTotalCost(const Graph& graph);
        int getNIterationAu();
        void printProprieties();
        void reset();

        Auction() { }

        Auction(int vertices, int scaling_factor = 1)
        {
			this->scaling_factor = scaling_factor;
            this->vertices = vertices;
            for (int i : boost::irange(0, vertices))
            {
                this->unassigned_bidder.insert(std::make_pair(i, Bidder{}));
                this->item_map.insert(std::make_pair(i, Item{}));
            }
        }
};


// Function that return the number of iterations 
template<typename Graph, typename Type>
inline int Auction<Graph, Type>::getNIterationAu() { return n_iteration_au; }


// Function to reset all the data structure except the cost attribute of all items
template<typename Graph, typename Type>
inline void Auction<Graph, Type>::reset()
{
    this->unassigned_bidder.clear();
    this->assigned_bidder.clear();
    for (int i : boost::irange(0, vertices))
    {       
        this->unassigned_bidder.insert(std::make_pair(i, Bidder{}));
        this->item_map[i].high_bidder = -1;
        this->item_map[i].high_bid = -1;
    }
}


// Function taht return the weighted sum of the Assignment Problem solution
template<typename Graph, typename Type>
inline Type Auction<Graph, Type>::getTotalCost(const Graph& graph)
{
    Type total_cost_auction = 0;
    for (int bidder = 0; bidder < vertices; ++bidder) 
        total_cost_auction += boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder, assigned_bidder[bidder].best_item + vertices, graph)).first);
    return total_cost_auction;
}


// Function to check if the given graph satisfy the proprieties of an Assignment Problem
template<typename Graph, typename Type>
bool Auction<Graph, Type>::is_assignment_problem(const Graph& graph)
{
    for (auto v1 : boost::make_iterator_range(boost::vertices(graph)))
    {
        AdjacencyIterator<Graph> ai, a_end;
        boost::tie(ai, a_end) = boost::adjacent_vertices(v1, graph);
        if (ai == a_end) return false;
        else
            for (auto v2 : boost::make_iterator_range(ai, a_end))
                if ((v1 < vertices && v2 < vertices) || (v1 > vertices && v2 > vertices))
                    return false;
    }

    return true;
}


// Function that print the proprieties of both items and bidders
template<typename Graph, typename Type>
inline void Auction<Graph, Type>::printProprieties()
{
    for (auto& bidder : assigned_bidder)
        std::cout << "|Bidder:" << bidder.first << "|Best item:" << bidder.second.best_item << "|Value first best item:" << bidder.second.val_first_best_item << "|Value second best item:" << bidder.second.val_second_best_item << "|\n";
    for (auto& item : item_map)
        std::cout << "|Item:" << item.first << "|Cost:" << item.second.cost << "|Higher bidder:" << item.second.high_bidder << "|Higher bid:" << item.second.high_bid << "|\n";
}


// Function that execute a single execution cicle of th Auction Algorithm
template<typename Graph, typename Type>
void Auction<Graph, Type>::auctionRound(const Graph& graph, const double& eps, const vertex_idMap<Graph>& V_Map, bool& err)
{

    // BIDDING PHASE

    for (auto& bidder : unassigned_bidder)
    {

        // Temporal variables
        int id_item1 = -1;
        double val_item1 = static_cast<double>(std::numeric_limits<Weight>::min());
        double val_item2 = static_cast<double>(std::numeric_limits<Weight>::min());

        AdjacencyIterator<Graph> ai, a_end;
        boost::tie(ai, a_end) = boost::adjacent_vertices(V_Map[bidder.first], graph);

        // For each adjacent vertices
        for (auto item : boost::make_iterator_range(ai, a_end))
        {
            // Calculate the difference between the edge weghts and the item costs
            double val = ((boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder.first, static_cast<int>(item), graph)).first)))
                - item_map[static_cast<int>(item) - vertices].cost;
            if (val > val_item1)
            {
                val_item2 = val_item1;
                val_item1 = val;
                id_item1 = static_cast<int>(item) - vertices;
            }
            else if (val > val_item2) val_item2 = val;
        }

        // Update the variables
        bidder.second.best_item = id_item1;
        bidder.second.val_second_best_item = val_item2;
        bidder.second.val_first_best_item = val_item1;

        // Compute the actual bid
        double bid = bidder.second.val_first_best_item - bidder.second.val_second_best_item + eps;

        if (item_map.find(bidder.second.best_item) != item_map.end())
        {
            // Update the item attribute in case the bid is higher than the actual bid
            if (bid > item_map[bidder.second.best_item].high_bid)
            {
                item_map[bidder.second.best_item].high_bid = bid;
                item_map[bidder.second.best_item].high_bidder = bidder.first;
			}
        }
		else
		{
            // Exit the execution since the bidder has not found any item that sute him
            err = true;
            break;
		}
    }


    if (!err)
    {

        // ASSIGNMENT PHASE

        for (auto& item : item_map)
        {
            // If the item has not obtained any bid go to the next one
            if (item.second.high_bid == -1) continue;

            item.second.cost += item.second.high_bid; // Update the item's cost 
            int id_to_remove = -1;

            // Check if any other assigned bidder has that item 
            for (auto& ass_bidr : assigned_bidder)
            {
                if (ass_bidr.second.best_item == item.first)
                {
                    id_to_remove = ass_bidr.first;
                    break;
                }
            }

            // In case remove it and reinsert it in the unassigned bidder map
            if (id_to_remove != -1)
            {
                unassigned_bidder.insert(std::make_pair(id_to_remove, assigned_bidder[id_to_remove]));
                assigned_bidder.erase(id_to_remove);
            }

            // Assign the examined item to its best bidder
            assigned_bidder.insert(std::make_pair(item.second.high_bidder, unassigned_bidder[item.second.high_bidder]));
            unassigned_bidder.erase(item.second.high_bidder);

        }
    }
    
}


// Function for the original Auction Algorithm
template<typename Graph, typename Type>
void Auction<Graph, Type>::original_auction(const Graph& graph, std::vector<int>& ass)
{
    if (!is_assignment_problem(graph)) throw("Not an assignment problem");

    vertex_idMap<Graph> V_Map = boost::get(boost::vertex_index, graph);

    double eps = 1.0 / (vertices + 1);
    bool err = false;

    while (unassigned_bidder.size() > 0 && !err)
    {
        auctionRound(graph, eps, V_Map, err);

        n_iteration_au += 1;
    }

    if(!err)
        for (auto& a : assigned_bidder) ass[a.first] = a.second.best_item;
    else
        for (int i = 0; i < ass.size(); ++i) ass[i] = -1;

}


// Function for the e-scaling variation of the Auction Algorithm
template<typename Graph, typename Type>
void Auction<Graph, Type>::e_scaling_auction(const Graph& graph, std::vector<int>& ass, const double& scaling_factor)
{
    if (!is_assignment_problem(graph)) throw("Not an assignment problem");
    int k = 0;
    vertex_idMap<Graph> V_Map = boost::get(boost::vertex_index, graph);

    double eps = 1.0;
    bool err = false;

    while (eps > 1.0 / vertices && !err)
    {
        reset();

        while (unassigned_bidder.size() > 0 && !err)
        {
            auctionRound(graph, eps, V_Map, err);

            n_iteration_au += 1;
        }

        eps = eps * scaling_factor;
        k += 1;
    }

    if (!err)
        for (auto& a : assigned_bidder) ass[a.first] = a.second.best_item;
    else
        for (int i = 0; i < ass.size(); ++i) ass[i] = -1;

}
#endif

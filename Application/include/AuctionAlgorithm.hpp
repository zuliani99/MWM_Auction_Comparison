#ifndef _AA_H
#define _AA_H

#include <vector>
#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>


template<typename T>
using AdjacencyIterator = boost::graph_traits<T>::adjacency_iterator;


template<typename Graph, typename Type>
class Auction
{
    private:
        struct Bidder {
            int best_item = -1;
            Type val_first_best_item = -1;
            Type val_second_best_item = -1;
        };

        struct Item {
            Type cost = 0;
            int high_bidder = -1;
            Type high_bid = -1;
        };

        int n_iteration_au = 0;
        int vertices = 0;

        std::unordered_map<int, Bidder> unassigned_bidder;
        std::unordered_map<int, Bidder> assigned_bidder;
        std::unordered_map<int, Item> item_map;
        
        bool is_assignment_problem(const Graph& graph);
        void auctionRound(const Graph& graph, const Type& eps, const auto& vertex_idMap);
        
    public:
        void auction_algorithm(const Graph& graph, std::vector<int>& ass);
        int getNIterationAu();
        Type getTotalCost(const Graph& graph);
        void printProprieties();
        Type getMaximumEdge(const Graph& graph);
        void reset();

        Auction(int vertices)
        {
            this->vertices = vertices;
            for (int i : boost::irange(0, vertices))
            {
                this->unassigned_bidder.insert(std::make_pair(i, Bidder{}));
                this->item_map.insert(std::make_pair(i, Item{}));
            }
        }
};


template<typename Graph, typename Type>
inline int Auction<Graph, Type>::getNIterationAu() { return n_iteration_au; }


template<typename Graph, typename Type>
void Auction<Graph, Type>::reset()
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


template<typename Graph, typename Type>
Type Auction<Graph, Type>::getMaximumEdge(const Graph& graph)
{
    Type max = 0;
    typedef boost::graph_traits<Graph>::edge_iterator edge_iterator;

    std::pair<edge_iterator, edge_iterator> ei = boost::edges(graph);
    for (edge_iterator edge_iter = ei.first; edge_iter != ei.second; ++edge_iter)
        if (boost::get(boost::edge_weight_t(), graph, *edge_iter) > max)
            max = boost::get(boost::edge_weight_t(), graph, *edge_iter);
        
    return max;
}


template<typename Graph, typename Type>
inline Type Auction<Graph, Type>::getTotalCost(const Graph& graph)
{
    Type total_cost_auction = 0;
    for (int bidder = 0; bidder < vertices; ++bidder) 
        total_cost_auction += boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder, assigned_bidder[bidder].best_item + vertices, graph)).first);
    return total_cost_auction;
}


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


template<typename Graph, typename Type>
inline void Auction<Graph, Type>::printProprieties()
{
    for (auto& bidder : assigned_bidder)
        std::cout << "|Bidder:" << bidder.first << "|Best item:" << bidder.second.best_item << "|Value first best item:" << bidder.second.val_first_best_item << "|Value second best item:" << bidder.second.val_second_best_item << "|\n";
    for (auto& item : item_map)
        std::cout << "|Item:" << item.first << "|Cost:" << item.second.cost << "|Higher bidder:" << item.second.high_bidder << "|Higher bid:" << item.second.high_bid << "|\n";
}


template<typename Graph, typename Type>
void Auction<Graph, Type>::auctionRound(const Graph& graph, const Type& eps, const auto& vertex_idMap)
{
    for (auto& bidder : unassigned_bidder)
    {

        int id_item1 = -1;
        Type val_item1 = -1;
        Type val_item2 = -1;

        AdjacencyIterator<Graph> ai, a_end;
        boost::tie(ai, a_end) = boost::adjacent_vertices(vertex_idMap[bidder.first], graph);

        for (auto item : boost::make_iterator_range(ai, a_end)) // itero iniziando da quelli che hanno meno vertici?
        {
            Type val = (boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder.first, static_cast<int>(item), graph)).first)) // * (vertices))
                - item_map[static_cast<int>(item) - vertices].cost;
            if (val > val_item1)
            {
                val_item2 = val_item1;
                val_item1 = val;
                id_item1 = static_cast<int>(item) - vertices;
            }
            else if (val > val_item2) val_item2 = val;
        }

        bidder.second.best_item = id_item1;
        bidder.second.val_second_best_item = val_item2;
        bidder.second.val_first_best_item = val_item1;

        Type bid = bidder.second.val_first_best_item - bidder.second.val_second_best_item + eps;

        if (item_map.find(bidder.second.best_item) != item_map.end())
        {
            if (bid > item_map[bidder.second.best_item].high_bid)
            {
                item_map[bidder.second.best_item].high_bid = bid;
                item_map[bidder.second.best_item].high_bidder = bidder.first;
            }
        }
        /*else
        {
            throw std::runtime_error("Loop");
        }*/

    }
    
    for (auto& item : item_map)
    {
        if (item.second.high_bid == -1) continue;

        item.second.cost += item.second.high_bid;
        int id_to_remove = -1;

        for (auto& ass_bidr : assigned_bidder)
        {
            if (ass_bidr.second.best_item == item.first)
            {
                id_to_remove = ass_bidr.first;
                break;
            }
        } 
                
        if (id_to_remove != -1)
        {
            unassigned_bidder.insert(std::make_pair(id_to_remove, assigned_bidder[id_to_remove]));
            assigned_bidder.erase(id_to_remove);
        }

        assigned_bidder.insert(std::make_pair(item.second.high_bidder, unassigned_bidder[item.second.high_bidder]));
        unassigned_bidder.erase(item.second.high_bidder);

    }
}


template<typename Graph, typename Type>
void Auction<Graph, Type>::auction_algorithm(const Graph& graph, std::vector<int>& ass)
{
    if (!is_assignment_problem(graph)) throw("Not an assignment problem");

    auto vertex_idMap = boost::get(boost::vertex_index, graph);

    /*Type eps = 0;
    Type k = 0;
    const Type C = getMaximumEdge(graph);

    // HYPERPARAMETERS
    const Type delta = (C / 5 + C / 2) / 2;
    const Type theta = 7.0;
    
    eps = std::max<Type>(static_cast<Type>(1), delta / std::pow(theta, k));*/

    Type eps = static_cast<Type>(1.0 / (vertices + 1));
    //Type eps = 1;

    /*while (eps >= 1)//(eps > 1.0 / vertices)
    {
        reset();*/

        while (unassigned_bidder.size() > 0)
        {
            auctionRound(graph, eps, vertex_idMap);

            n_iteration_au += 1;
        }

        /*k += 1;
        eps = (delta / std::pow(theta, k));
        eps *= 0.25;
    }*/

    for (auto& a : assigned_bidder) ass[a.first] = a.second.best_item;

}

#endif

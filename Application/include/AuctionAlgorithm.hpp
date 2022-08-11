#ifndef _AA_H
#define _AA_H

#include <vector>
#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>

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
        
    public:
        void auction_algorithm(const Graph& graph, const Type eps, std::vector<int>& ass);
        int getNIterationAu();
        Type getTotalCost(const Graph& graph);
        void printProprieties();

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
int Auction<Graph, Type>::getNIterationAu() { return n_iteration_au; }


template<typename Graph, typename Type>
Type Auction<Graph, Type>::getTotalCost(const Graph& graph)
{
    Type total_cost_auction = 0;
    for (int bidder = 0; bidder < vertices; ++bidder) 
        total_cost_auction += boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder, assigned_bidder[bidder].best_item + vertices, graph)).first);
    return total_cost_auction;
}

template<typename Graph, typename Type>
bool Auction<Graph, Type>::is_assignment_problem(const Graph& graph)
{
    using AdjacencyIterator = boost::graph_traits<Graph>::adjacency_iterator;
    using vertex_iterator = Graph::vertex_iterator;

    vertex_iterator i, end;
    for (auto v1 : boost::make_iterator_range(boost::vertices(graph)))
    {
        AdjacencyIterator ai, a_end;
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
void Auction<Graph, Type>::auction_algorithm(const Graph& graph, const Type eps, std::vector<int>& ass)
{
    if (!is_assignment_problem(graph)) throw("Not an assignment problem");

    /*struct Bidder {
        int best_item = -1;
        Type val_first_best_item = -1;
        Type val_second_best_item = -1;
    };

    struct Item {
        Type cost = 0;
        int high_bidder = -1;
        Type high_bid = -1;
    };

    std::unordered_map<int, Bidder> unassigned_bidder;
    std::unordered_map<int, Bidder> assigned_bidder;
    std::unordered_map<int, Item> item_map;
    for(int i : boost::irange(0, vertices))
    {
        unassigned_bidder.insert(std::make_pair(i, Bidder{}));
        item_map.insert(std::make_pair(i, Item{}));
    }*/


    while (unassigned_bidder.size() > 0)
    {
        for (auto& bidder : unassigned_bidder)
        {

            int id_item1 = -1;
            Type val_item1 = -1;
            Type val_item2 = -1;

            for (auto& item : item_map)
            {   
                //if (boost::edge(bidder.first, item.first + vertices, graph).second)
                //{
                    Type val = boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder.first, item.first + vertices, graph)).first) - item.second.cost;
                    if (val > val_item1)
                    {
                        val_item2 = val_item1;
                        val_item1 = val;
                        id_item1 = item.first;
                    }
                    else if (val > val_item2) val_item2 = val;
                //}
            }

            bidder.second.best_item = id_item1;
            bidder.second.val_second_best_item = val_item2;
            bidder.second.val_first_best_item = val_item1;

            Type bid = bidder.second.val_first_best_item - bidder.second.val_second_best_item + eps;

            if (bid > item_map[bidder.second.best_item].high_bid)
            {
                item_map[bidder.second.best_item].high_bid = bid;
                item_map[bidder.second.best_item].high_bidder = bidder.first;
            }

        }

        for (auto& item : item_map)
        {

            if (item.second.high_bid == -1) continue;
            item.second.cost += item.second.high_bid;

            std::vector<int> id_to_remove;


            for (auto& ass_bidr : assigned_bidder)
                if (ass_bidr.second.best_item == item.first)
                    id_to_remove.push_back(ass_bidr.first);

            for (int id : id_to_remove)
            {
                unassigned_bidder.insert(std::make_pair(id, assigned_bidder[id]));
                assigned_bidder.erase(id);
            }
            assigned_bidder.insert(std::make_pair(item.second.high_bidder, unassigned_bidder[item.second.high_bidder]));
            unassigned_bidder.erase(item.second.high_bidder);
        }

        n_iteration_au += 1;
    }    

    for (auto& a : assigned_bidder) ass[a.first] = a.second.best_item;

}

template<typename Graph, typename Type>
void Auction<Graph, Type>::printProprieties() 
{
    for (auto& bidder : assigned_bidder)
        std::cout << "|Bidder:" << bidder.first << "|Best item:" << bidder.second.best_item << "|Value first best item:" << bidder.second.val_first_best_item << "|Value second best item:" << bidder.second.val_second_best_item << "|\n";
    for (auto& item : item_map)
        std::cout << "|Item:" << item.first << "|Cost:" << item.second.cost << "|Higher bidder:" << item.second.high_bidder << "|Higher bid:" << item.second.high_bid << "|\n";
}

#endif


/*template<typename Graph, typename Type>
void auction_algorithm(Graph& graph, Type eps, int vertices, int& n_iteration_au, std::vector<int>& assignments)
{
    if (!is_assignment_problem(graph, vertices)) throw("Not an assignment problem");

    size_t unassigned_bidders = vertices;

    std::vector<Type> cost(vertices, 0), highBids(vertices, static_cast<Type>(-1)), firstBestVal(vertices, static_cast<Type>(-1)), secondBestVal(vertices, static_cast<Type>(-1));
    std::vector<int> highBidder(vertices, -1), item2bidder(vertices, -1), IDFirstBestItem(vertices, -1);

    while (unassigned_bidders > 0)
    {
        for (int bidder = 0; bidder < vertices; bidder++) // for each unassigned bidder
        {
            if (assignments[bidder] != -1) continue;

            int id_item1 = -1;
            Type val_item1 = -1;
            Type val_item2 = -1;

            for (int item = 0; item < vertices; item++)
            {
                Type val = boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder, item + vertices, graph)).first) - cost[item];
                if (val > val_item1)
                {
                    val_item2 = val_item1;
                    val_item1 = val;
                    id_item1 = item;
                }
                else if (val > val_item2)
                    val_item2 = val;
            }

            IDFirstBestItem[bidder] = id_item1;
            secondBestVal[bidder] = val_item2;
            firstBestVal[bidder] = val_item1;

            Type bid = firstBestVal[bidder] - secondBestVal[bidder] + eps;
            if (bid > highBids[IDFirstBestItem[bidder]])
            {
                highBids[IDFirstBestItem[bidder]] = bid;
                highBidder[IDFirstBestItem[bidder]] = bidder;
            }

        }

        for (int item = 0; item < vertices; item++)
        {
            if (highBids[item] == -1) continue;

            cost[item] += highBids[item];

            if (item2bidder[item] != -1) {
                assignments[item2bidder[item]] = -1;
                unassigned_bidders++;
            }

            item2bidder[item] = highBidder[item];
            assignments[highBidder[item]] = item;
            unassigned_bidders--;
        }

        n_iteration_au += 1;
    }

}*/
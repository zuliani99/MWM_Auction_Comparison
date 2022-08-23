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

        std::unordered_map<int, Bidder> unassigned_bidder;
        std::unordered_map<int, Bidder> assigned_bidder;
        std::unordered_map<int, Item> item_map;
        
        bool is_assignment_problem(const Graph& graph);
        void auctionRound(const Graph& graph, const double& eps, const vertex_idMap<Graph>& V_Map);
        
    public:
		void naive_auction(const Graph& graph, std::vector<int>& ass);
        void e_scaling(const Graph& graph, std::vector<int>& ass);
        //void e_scaling_with_C(const Graph& graph, std::vector<int>& ass);
        int getNIterationAu();
        Type getTotalCost(const Graph& graph);
        void printProprieties();
        Type getMaximumEdge(const Graph& graph);
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

template<typename Graph, typename Type>
inline int Auction<Graph, Type>::getNIterationAu() { return n_iteration_au; }


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


template<typename Graph, typename Type>
inline Type Auction<Graph, Type>::getMaximumEdge(const Graph& graph)

{
    Type max = 0;

    std::pair<edge_iterator<Graph>, edge_iterator<Graph>> ei = boost::edges(graph);
    for (edge_iterator<Graph> edge_iter = ei.first; edge_iter != ei.second; ++edge_iter)
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
void Auction<Graph, Type>::auctionRound(const Graph& graph, const double& eps, const vertex_idMap<Graph>& V_Map)
{
    for (auto& bidder : unassigned_bidder)
    {

        int id_item1 = -1;
        double val_item1 = static_cast<double>(std::numeric_limits<Weight>::min());
        double val_item2 = static_cast<double>(std::numeric_limits<Weight>::min());

        AdjacencyIterator<Graph> ai, a_end;
        boost::tie(ai, a_end) = boost::adjacent_vertices(V_Map[bidder.first], graph);

        for (auto item : boost::make_iterator_range(ai, a_end))
        {
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

        bidder.second.best_item = id_item1;
        bidder.second.val_second_best_item = val_item2;
        bidder.second.val_first_best_item = val_item1;

        double bid = bidder.second.val_first_best_item - bidder.second.val_second_best_item + eps;

        if (item_map.find(bidder.second.best_item) != item_map.end())
        {
            if (bid > item_map[bidder.second.best_item].high_bid)
            {
                item_map[bidder.second.best_item].high_bid = bid;
                item_map[bidder.second.best_item].high_bidder = bidder.first;
			}
        }
		//else
		//{
			//std::cout << " loop\n";
		//}
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
void Auction<Graph, Type>::naive_auction(const Graph& graph, std::vector<int>& ass)
{
    if (!is_assignment_problem(graph)) throw("Not an assignment problem");

    vertex_idMap<Graph> V_Map = boost::get(boost::vertex_index, graph);

    double eps = 1.0 / (vertices + 1);


    while (unassigned_bidder.size() > 0)
    {
        auctionRound(graph, eps, V_Map); /* * scaling_factor*/

        n_iteration_au += 1;
    }

    for (auto& a : assigned_bidder) ass[a.first] = a.second.best_item;

}


/*template<typename Graph, typename Type>
void Auction<Graph, Type>::e_scaling_with_C(const Graph& graph, std::vector<int>& ass)
{
    if (!is_assignment_problem(graph)) throw("Not an assignment problem");

    vertex_idMap<Graph> V_Map = boost::get(boost::vertex_index, graph);

    //double eps = 1.0 / (vertices + 1);

	int k = 0;
	double eps = 0.0;
	const double C = static_cast<double>(getMaximumEdge(graph));

	const double delta = (C / 5.0 + C / 2.0) / 2.0;
	const double theta = 7.0;

	eps = std::max(1.0, delta / std::pow(theta, k));

	while(eps > 1.0)
	{
		reset();

		while (unassigned_bidder.size() > 0)
		{
			auctionRound(graph, eps, V_Map);

			n_iteration_au += 1;
		}

		k += 1;
		eps = (delta / std::pow(theta, k)); 
	}

	if(k > 1) std::cout << " KKKKKKKKKKKKKKKKKKKKKKKK" << k << "\n";

    for (auto& a : assigned_bidder) ass[a.first] = a.second.best_item;

}*/



template<typename Graph, typename Type>
void Auction<Graph, Type>::e_scaling(const Graph& graph, std::vector<int>& ass)
{
    if (!is_assignment_problem(graph)) throw("Not an assignment problem");
    int k = 0;
    vertex_idMap<Graph> V_Map = boost::get(boost::vertex_index, graph);

    double eps = 1.0;

    while (eps >= 1.0 / vertices)
    {
        reset();

        while (unassigned_bidder.size() > 0)
        {
            auctionRound(graph, eps, V_Map); // * scaling_factor

            n_iteration_au += 1;
        }

        eps = eps * .1;
        k += 1;
    }

    //if (k > 1)
        //std::cout << " okokokokokokokokokokokokokokokokokokokokokokokokokokokokokokokokokokokokok " << k << "\n";

    for (auto& a : assigned_bidder) ass[a.first] = a.second.best_item;

}
#endif

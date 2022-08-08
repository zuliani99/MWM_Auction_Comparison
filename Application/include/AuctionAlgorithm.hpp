#ifndef _AA_H
#define _AA_H

#include <vector>
#include <boost/graph/adjacency_list.hpp>

template<typename Graph>
bool is_assignment_problem(Graph& graph, int vertices)
{
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
void auction_algorithm(Graph& graph, Type eps, std::vector<int>& assignments, int vertices, int& n_iteration_au)
{
    //if (!is_assignment_problem(graph, vertices)) throw("Not an assignment problem");
    size_t unassigned_bidders = vertices;

    std::vector<Type> cost(vertices, 0), highBids(vertices, Type(-1)), firstBestVal(vertices, Type(-1)), secondBestVal(vertices, Type(-1));
    std::vector<int> highBidder(vertices, -1), item2bidder(vertices, -1), IDFirstBestItem(vertices, -1);

    while (unassigned_bidders > 0)
    {
        for (int bidder = 0; bidder < vertices; bidder++)
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
        }

        n_iteration_au += 1;
    }
}

#endif
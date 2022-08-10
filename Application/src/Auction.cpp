#include "../include/Auction.h"
#include "../include/BipartiteGraph.h"
#include "../include/AuctionAlgorithm.hpp"

/*void auction_algorithm_bundle_prop(Graph& graph, const int& n, Duration& elapsed, int& n_iteration_au) {
    const Weight eps = (1 / n) * 10'000;
    int unassigned_bidders = n;
    GraphProp& gp = graph[boost::graph_bundle];

	auto const id_bidders = boost::irange(0, n);
	auto const id_items   = boost::irange(n, 2 * n);
    auto t_start = now();

    while (unassigned_bidders > 0) {

        for (int id_bidder : id_bidders) {
            if (gp.bidder2item[static_cast<int>(id_bidder)] != -1) continue;
            Bidder* bidder = boost::get<Bidder>(&graph[id_bidder]);

            
            // 1 Bid

            int id_item1 = -1;
            Weight val_item1 = -1;
            Weight val_item2 = -1;

			//auto verticies_items = boost::adjacent_vertices(id_bidder, graph);
			
            for (int id_item : id_items) {
                Item* item = boost::get<Item>(&graph[static_cast<int>(id_item)]);
                Weight val = boost::get(boost::edge_weight_t(), graph, (boost::edge(id_bidder, id_item, graph)).first) - item->cost;

                if (val > val_item1) {
                    val_item2 = val_item1;
                    val_item1 = val;
                    id_item1 = item->id;
                }
                else if (val > val_item2) {
                    val_item2 = val;
                }
            }

            bidder->best_item = id_item1 + n;
            bidder->val_first_best_item = val_item1;
            bidder->val_second_best_item = val_item2;


            // 2 Compete

            Weight bid = bidder->val_first_best_item - bidder->val_second_best_item + eps;
            auto best_item = boost::get<Item>(&graph[bidder->best_item]);
            if (bid > best_item->high_bid) {
                best_item->high_bid = bid;
                best_item->high_bidder = bidder->id;
            }

        }


        // 3 Assign

        for (int id_item : id_items) {
            Item* item = boost::get<Item>(&graph[id_item]);
            if (item->high_bid == -1) continue;

            item->cost += item->high_bid;

            if (gp.item2bidder[item->id] != -1) {
                gp.bidder2item[gp.item2bidder[item->id]] = -1;
                unassigned_bidders++;
            }

            gp.item2bidder[item->id] = item->high_bidder;
            gp.bidder2item[gp.item2bidder[item->id]] = item->id;
            unassigned_bidders--;
        }

		n_iteration_au += 1;
    
    }

    elapsed = now() - t_start;
}*/



Weight perform_au(Graph& graph, Duration& elapsed, int& n_iteration_au, bool verbose)
{
    int n = int(boost::num_vertices(graph) / 2);
    //Weight total_cost_auction = 0;
    //std::vector<int> assignments(n ,-1);
    std::vector<int> assignments(n);

    //auction_algorithm_bundle_prop(graph, n, elapsed, n_iteration_au);


    //auto t_start = now();
    //auction_algorithm2<Graph, Weight>(graph, Weight((1 / n) * 10'000), n_iteration_au, assignments);
    //elapsed = now() - t_start;

    Auction<Graph, Weight> auction_problem(n);

    auto t_start = now();
    auction_problem.auction_algorithm(graph, static_cast<Weight>((1 / n) * 10'000), assignments);
    elapsed = now() - t_start;

    std::cout << " Finished \nThe matching is: ";
    for (int bidder = 0; bidder < n; ++bidder)
    {
        std::cout << "(" << bidder << "," << assignments[bidder] << ")";
        //total_cost_auction += boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder, assignments[bidder] + n, graph)).first);
        //std::cout << "(" << bidder << "," << graph[boost::graph_bundle].bidder2item[bidder] << ")";
        //int item = graph[boost::graph_bundle].bidder2item[bidder];
        //total_cost_auction += boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder, item + n, graph)).first);
    }
    std::cout << "\n";

    if (verbose) auction_problem.printProprieties();
    n_iteration_au = auction_problem.getNIterationAu();

    return auction_problem.getTotalCost(graph);
}
#include "Auction.h"
#include "BipartiteGraph.h"

void auction_algorithm(Graph& graph, const int& n, duration& elapsed) {
    const Weight eps = 1;
    int unassigned_bidders = n;
    GraphProp& gp = graph[boost::graph_bundle];

    EdgeFilter any_interconnect = boost::keep_all{};
    VertexFilter bidders = [graph](V v) -> bool { return boost::get<Bidder>(&(graph)[v]); };
    VertexFilter items = [graph](V v) -> bool { return boost::get<Item>(&(graph)[v]); };

    FMap map_bidders = FMap(graph, any_interconnect, bidders);
    FMap map_items = FMap(graph, any_interconnect, items);    
    
    auto t_start = now();
    
    while (unassigned_bidders > 0) {


        // 1 Bid

        for (auto uncasted_bidder : boost::make_iterator_range(boost::vertices(map_bidders))) {
            Bidder* bidder = boost::get<Bidder>(&graph[uncasted_bidder]);
            if(gp.bidder2item[bidder->id] != -1) continue;

            int id_item1 = -1;
            Weight val_item1 = -1;
            Weight val_item2 = -1;

            for (auto uncasted_item : boost::make_iterator_range(boost::vertices(map_items))) {
                Item* item = boost::get<Item>(&graph[uncasted_item]);
                Weight weight = boost::get(boost::edge_weight_t(), graph, (boost::edge(uncasted_bidder, uncasted_item, graph)).first);
                Weight val = weight - item->cost;

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

        }


        // 2 Compete

        for (auto uncasted_bidder : boost::make_iterator_range(boost::vertices(map_bidders))) {
            Bidder* bidder = boost::get<Bidder>(&graph[uncasted_bidder]);
            if (gp.bidder2item[bidder->id] != -1) continue;

            Weight bid = bidder->val_first_best_item - bidder->val_second_best_item + eps;
            auto best_item = boost::get<Item>(&graph[bidder->best_item]);
            if (bid > best_item->high_bid) {
                best_item->high_bid = bid;
                best_item->high_bidder = bidder->id;
            }

        }


        // 3 Assign

        for (auto uncasted_item : boost::make_iterator_range(boost::vertices(map_items))) {
            Item* item = boost::get<Item>(&graph[uncasted_item]);
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
    
    }

    elapsed = now() - t_start;
}

Weight perform_au(Graph& graph, duration& elapsed) {
    int n = int(boost::num_vertices(graph) / 2);
    Weight total_cost_auction = 0;

    auction_algorithm(graph, n, elapsed);

    std::cout << "The matching is:";
    for (int bidder = 0; bidder < n; ++bidder) {
        std::cout << "(" << bidder << "," << graph[boost::graph_bundle].bidder2item[bidder] << ")";
        int item = graph[boost::graph_bundle].bidder2item[bidder];
        total_cost_auction += boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder, item + n, graph)).first);
    }
    std::cout << "\n";
    return total_cost_auction;
}
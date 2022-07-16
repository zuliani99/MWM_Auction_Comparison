#include "Auction.h"

std::vector<int> auction_algorithm(std::vector<std::vector<float>>* cost_matrix, const int *n_vertices_per_part, long long *time_execution) {
	const int n_bidders = *n_vertices_per_part;
	const int n_items = *n_vertices_per_part;
	const double eps = 0.1;
    int unassigned_bidders = n_bidders;

    std::unordered_map<int, Bidder> Bidders = retunr_n<Bidder>(n_bidders);
    std::unordered_map<int, Item> Items = retunr_n<Item>(n_items);
    std::vector<int>bidder2item(n_bidders, -1);
    std::vector<int>item2bidder(n_bidders, -1);
 
    auto t_start = std::chrono::high_resolution_clock::now();

    while (unassigned_bidders > 0) {

        std::cout << unassigned_bidders << std::endl;
        // --
        // Bid

        for (auto bidder = Bidders.begin(); bidder != Bidders.end(); ++bidder) {
            int idx1_ = -1; // index of highest payoff item
            double val1_ = -1; // first highest payoff
            double val2_ = -1; // second highest payoff

            for (auto item = Items.begin(); item != Items.end(); ++item) {
                double val = (*cost_matrix)[bidder->first][item->first] - item->second.cost; // A_ij - p_j
                if (val > val1_) {
                    val2_ = val1_;
                    val1_ = val;
                    idx1_ = item->first;
                }
                else if (val > val2_) val2_ = val;
            }
            bidder->second.idx_first_item = idx1_;
            bidder->second.first_item = val1_;
            bidder->second.second_item = val2_;
        }


        // --
        // Compete

        for (auto bidder = Bidders.begin(); bidder != Bidders.end(); ++bidder) {
            double bid = bidder->second.first_item - bidder->second.second_item + eps;  // (A_ij - p_j) - (A_ik - p_k) + e
            if (bid > Items.find(bidder->second.idx_first_item)->second.high_bidder) {
                Items.find(bidder->second.idx_first_item)->second.high_bid = bid;
                Items.find(bidder->second.idx_first_item)->second.high_bidder = bidder->first;
                // sostituisxco il bidder con l'attuale
            }
        }


        // --
        // Assign

        for (auto item = Items.begin(); item != Items.end(); ++item) {
            item->second.cost += item->second.high_bid;

            if (item2bidder[item->first] != -1) {
                bidder2item[item2bidder[item->first]] = -1;
                unassigned_bidders++;
            }

            item2bidder[item->first] = item->second.high_bidder;
            bidder2item[item->second.high_bidder] = item->first;
            unassigned_bidders--;
        }
    }

    //std::cout << "loop_counter=%d | " << loop_counter << std::endl;
    *time_execution = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t_start).count();
    return bidder2item;
}
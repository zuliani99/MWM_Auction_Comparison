#include"Auction_Array.h"

void auction(Matrix& cost_matrix, const int& n, duration& elapsed, std::vector<int>& bidder2item) {

    const Weight eps = 1;

    std::vector<Weight> cost(n, 0);
    std::vector<Weight> high_bids(n, -1);
    std::vector<Weight> val1(n, -1);
    std::vector<Weight> val2(n, -1);

    std::vector<int> high_bidder(n, -1);
    std::vector<int> idx1(n, -1);
    std::vector<int> item2bidder(n, -1);

    int unassigned_bidders = n;

    auto t_start = now();

    while (unassigned_bidders > 0) {

        for (int bidder = 0; bidder < n; bidder++) {
            if (bidder2item[bidder] != -1) continue;

            int idx1_ = -1;
            Weight val1_ = -1;
            Weight val2_ = -1; 

            for (int item = 0; item < n; item++) {
                Weight val = cost_matrix[bidder][item] - cost[item]; // A_ij - p_j
                if (val > val1_) {
                    val2_ = val1_;
                    val1_ = val;
                    idx1_ = item;
                }
                else if (val > val2_) {
                    val2_ = val;
                }
            }

            idx1[bidder] = idx1_;
            val2[bidder] = val2_;
            val1[bidder] = val1_;


            // Copmete

            Weight bid = val1[bidder] - val2[bidder] + eps;
            if (bid > high_bids[idx1[bidder]]) {
                high_bids[idx1[bidder]] = bid;
                high_bidder[idx1[bidder]] = bidder;
            }

        }

        // Assign

        for (int item = 0; item < n; item++) {
            if (high_bids[item] == -1) continue;

            cost[item] += high_bids[item];

            if (item2bidder[item] != -1) { 
                bidder2item[item2bidder[item]] = -1;
                unassigned_bidders++;
            }

            item2bidder[item] = high_bidder[item]; 
            bidder2item[high_bidder[item]] = item;
            unassigned_bidders--;
        }
    }
    elapsed = now() - t_start;
}

Weight perform_au_array(Matrix& cost_matrix, int& n, duration& elapsed) {
    Weight total_cost_auction = 0;
    std::vector<int> bidder2item(n, -1);

    auction(cost_matrix, n, elapsed, bidder2item);

    std::cout << "\nThe matchingg is: ";
    for (int bidder = 0; bidder < n; ++bidder) {
        std::cout << "(" << bidder << "," << bidder2item[bidder] << ")";
        total_cost_auction += cost_matrix[bidder][bidder2item[bidder]];
    }
    std::cout << "\n";
    return total_cost_auction;
}
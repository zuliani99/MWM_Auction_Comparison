#include "Auction.h"
#include <chrono>
#include "omp.h"


template<typename Val, typename Int>
void fill(Val* x, Int n, Val val) {
    for (Int i = 0; i < n; i++) x[i] = val;
}

void auction_algorithm(double* cost_matrix, int n_vertices_per_part, int* bidder2item) {
	const int n_bidders = n_vertices_per_part;
	const int n_items = n_vertices_per_part;
	const double eps = 0.1;


    double* cost = (double*)malloc(n_items * sizeof(double));
    double* high_bids = (double*)malloc(n_items * sizeof(double));
    int* high_bidder = (int*)malloc(n_items * sizeof(int));

    int* item2bidder = (int*)malloc(n_bidders * sizeof(int));
    

    fill<double>(cost, n_items, 0);
    fill<double>(high_bids, n_items, -1);
    fill<int>(high_bidder, n_items, -1);
    fill<int>(item2bidder, n_items, -1);
    fill<int>(bidder2item, n_bidders, -1);

    int* idx1 = (int*)malloc(n_bidders * sizeof(int));
    double* val1 = (double*)malloc(n_bidders * sizeof(double));
    double* val2 = (double*)malloc(n_bidders * sizeof(double));

    int unassigned_bidders = n_bidders;

    int loop_counter = 0;

    fill<int>(idx1, n_bidders, -1);
    fill<double>(val1, n_bidders, -1);
    fill<double>(val2, n_bidders, -1);

    while (unassigned_bidders > 0) {
        loop_counter += unassigned_bidders;
        for (int bidder = 0; bidder < n_bidders; bidder++) {
            if (bidder2item[bidder] != -1) continue;

            int idx1_ = -1; // index of highest payoff item
            double val1_ = -1; // first highest payoff
            double val2_ = -1; // second highest payoff

            for (int item = 0; item < n_items; item++) {
                double val = cost_matrix[n_bidders * bidder + item] - cost[item]; // A_ij - p_j
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
        }
    }

    fill<double>(high_bids, n_items, -1);
    fill<int>(high_bidder, n_items, -1);

    for (int bidder = 0; bidder < n_bidders; bidder++) {
        if (bidder2item[bidder] != -1) continue;

        double bid = val1[bidder] - val2[bidder] + eps; // (A_ij - p_j) - (A_ik - p_k) + e
        if (bid > high_bids[idx1[bidder]]) {
            high_bids[idx1[bidder]] = bid;
            high_bidder[idx1[bidder]] = bidder;
        }
    }

    for (int item = 0; item < n_items; item++) {
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
#ifndef __AUCTION_H__
#define __AUCTION_H__

#include <malloc.h>
#include <chrono>
#include "omp.h"

template<typename Val>
void fill(Val* x, int n, int val) {
    for (int i = 0; i < n; i++) x[i] = val;
}


long long auction(double* cost_matrix, int n_bidders, int n_items, double eps, int* bidder2item) {
    auto t_start = std::chrono::high_resolution_clock::now();


    // Features of items
    double* cost = (double*)malloc(n_items * sizeof(double));
    double* high_bids = (double*)malloc(n_items * sizeof(double));
    int* high_bidder = (int*)malloc(n_items * sizeof(int));
    int* item2bidder = (int*)malloc(n_items * sizeof(int));

    //operazioni di fill 
    fill<double>(cost, n_items, 0);
    fill<double>(high_bids, n_items, -1);
    fill<int>(high_bidder, n_items, -1);
    fill<int>(item2bidder, n_items, -1);



    fill<int>(bidder2item, n_bidders, -1);



    // Features of Bidders
    int* idx1 = (int*)malloc(n_bidders * sizeof(int));
    //int* idx2 = (int*)malloc(n_bidders * sizeof(int));
    double* val1 = (double*)malloc(n_bidders * sizeof(double));
    double* val2 = (double*)malloc(n_bidders * sizeof(double));

    fill<int>(idx1, n_bidders, -1);
    //fill<int>(idx2, n_bidders, -1);
    fill<double>(val1, n_bidders, -1);
    fill<double>(val2, n_bidders, -1);

    fill<double>(high_bids, n_items, -1);
    fill<int>(high_bidder, n_items, -1);


    int unassigned_bidders = n_bidders;

    int loop_counter = 0;


    while (unassigned_bidders > 0) {


        loop_counter += unassigned_bidders;

        // --
        // Bid

        // This definitely helps in early iterations. May add overhead in later iterations
        // Might also be a good idea to use a different datastructure, to avoid iterating over _all_ bidders in later stages

        //sarebbe figo poter scorrere solo epr i bidder che non hanno un oggetto assegnato
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

        // --
        // Compete



        //per ogni bidder che non ha un oggetto assegnato
        for (int bidder = 0; bidder < n_bidders; bidder++) {
            if (bidder2item[bidder] != -1) continue;

            double bid = val1[bidder] - val2[bidder] + eps; // (A_ij - p_j) - (A_ik - p_k) + e
            if (bid > high_bids[idx1[bidder]]) {
                high_bids[idx1[bidder]] = bid;
                high_bidder[idx1[bidder]] = bidder; // sostituisco il bidder con l'attuale
            }
        }

        // --
        // Assign

        for (int item = 0; item < n_items; item++) {
            if (high_bids[item] == -1) continue;

            cost[item] += high_bids[item];  // p_j = p_j + ((A_ij - p_j) - (A_ik - p_k) + e)

            if (item2bidder[item] != -1) { // se l'oggetto item e' stato gia' assegnato ad un bidder
                bidder2item[item2bidder[item]] = -1; // annullo l'assegnamento
                unassigned_bidders++;
            }

            item2bidder[item] = high_bidder[item];  // assegno lo stesso oggetto al piu' grande bidder
            bidder2item[high_bidder[item]] = item; // assegnao al bidder l'oggetto
            unassigned_bidders--;
        }
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    //std::cout << "loop_counter=%d | " << loop_counter << std::endl;
    //return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t_start).count();
    double time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count();
    return time_taken;
}

#endif
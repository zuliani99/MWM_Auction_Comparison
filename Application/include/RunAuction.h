#ifndef _RunAuction_H
#define _RunAuction_H

#include "AuctionAlgorithm.hpp"

class RunAuction
{
public:
    Auction<Graph, Weight> auction;
    std::vector<int> assignments;
    Weight cost = 0;
    int iterations = 0;
    Duration elapsed;
    RunAuction(int n)
    {
        this->auction = Auction<Graph, Weight>(n);
        this->assignments = std::vector<int>(n, -1);
    }
};
#endif
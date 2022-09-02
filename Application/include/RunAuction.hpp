#ifndef _RunAuction_H
#define _RunAuction_H

#include "AuctionAlgorithm.hpp"

// Class used to store the a run of a Auction Algorithm (Original version or e-Scaling versions)
class RunAuction
{
public:
    Auction<Graph, Weight> auction;
    std::vector<int> assignments;
    Weight cost = 0;
    int iterations = 0;
	double scaling_factor;
    Duration elapsed;
    RunAuction(int n, int scaling_factor = 1)
    {
        this->auction = Auction<Graph, Weight>(n);
        this->assignments = std::vector<int>(n, -1);
		this->scaling_factor = (1.0 / scaling_factor); 
    }
};

#endif
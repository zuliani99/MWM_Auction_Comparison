#include "../include/Auction.h"
#include "../include/BipartiteGraph.h"
#include "../include/AuctionAlgorithm.hpp"


Weight perform_au(const Graph& graph, Duration& elapsed, int& n_iteration_au, const char* verbose)
{
    int n = int(boost::num_vertices(graph) / 2);
    bool solved = true;
    std::vector<int> assignments(n, -1);

    Auction<Graph, Weight> auction_problem(n);
    auto t_start = now();
    auction_problem.auction_algorithm(graph, assignments);
    elapsed = now() - t_start;

    for (int bidder = 0; bidder < n; ++bidder)
        if (boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder, assignments[bidder] + n, graph)).first) == 0)
        {
            solved = false;
            break;
        }

    if (!solved)
    {
        std::cout << " Finished \nNo matching found\n";
        if (*verbose == '1') auction_problem.printProprieties();
        n_iteration_au = -1;

        return static_cast<Weight>(-1);
    }
    else
    {
        /* (FROM vertex, TO vertex, Verex WEIGHT )*/
        std::cout << " Finished \nThe matching is: ";

        for (int bidder = 0; bidder < n; ++bidder)
            std::cout << "(" << bidder << "," << assignments[bidder] << "," << (boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder, assignments[bidder] + n, graph)).first)) / 10'000.0 << ")";
        std::cout << "\n";

        if (*verbose == '1') auction_problem.printProprieties();
        n_iteration_au = auction_problem.getNIterationAu();

        return auction_problem.getTotalCost(graph);
    }

    
}
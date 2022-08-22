#include "../include/Auction.h"
#include "../include/BipartiteGraph.h"
#include "../include/AuctionAlgorithm.hpp"

std::pair<std::string, Weight> perform_au(const Graph& graph, Duration& elapsed, int& n_iteration_au, const int& verbose)
{
    int n = int(boost::num_vertices(graph) / 2);
    bool solved = true;
    std::vector<int> assignments_1(n, -1);
    std::vector<int> assignments_2(n, -1);

    Auction<Graph, Weight> auction_problem_1(n/*, 10'000*/);
    Auction<Graph, Weight> auction_problem_2(n/*, 10'000*/);

    auto t_start_1 = now();
    auction_problem_1.auction_algorithm_1(graph, assignments_1);
    elapsed = now() - t_start_1;

    auto t_start_2 = now();
    auction_problem_2.auction_algorithm_2(graph, assignments_2);
    auto elapsed_2 = now() - t_start_2;

    if (std::find(assignments_1.begin(), assignments_1.end(), -1) != assignments_1.end()) { solved = false; }
        
    if (!solved)
    {
        std::cout << " Finished \nNo matching found\n";
        if (verbose) auction_problem_1.printProprieties();
        n_iteration_au = -1;

        return std::make_pair<std::string, Weight>("none", static_cast<Weight>(-1));
    }
    else
    {
        /* (FROM vertex, TO vertex, Verex WEIGHT )*/
        std::cout << " Finished \nThe matching is: ";

        //for (int bidder = 0; bidder < n; ++bidder)
            //std::cout << "(" << bidder << "," << assignments_1[bidder] << "," << (boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder, assignments_1[bidder] + n, graph)).first)) /*/ 10'000.0*/ << ")";
        //std::cout << "\n";

        //if (verbose) auction_problem.printProprieties();
        //n_iteration_au = auction_problem_1.getNIterationAu();

        std::cout << auction_problem_1.getTotalCost(graph) << "     " << auction_problem_2.getTotalCost(graph) << "\n";

        if (auction_problem_1.getTotalCost(graph) > auction_problem_2.getTotalCost(graph))
        {
            n_iteration_au = auction_problem_1.getNIterationAu();
            return std::make_pair<std::string, Weight>("epsilon_scaling", auction_problem_1.getTotalCost(graph));
        }
            
        else
        {
            n_iteration_au = auction_problem_2.getNIterationAu();
            return std::make_pair<std::string, Weight>("standard_auction", auction_problem_2.getTotalCost(graph));
        }
        
    }

    
}
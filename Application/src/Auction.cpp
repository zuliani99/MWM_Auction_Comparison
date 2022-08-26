#include "../include/Auction.h"
#include "../include/BipartiteGraph.h"
#include "../include/RunAuction.h"


// Fucntion that manage the execution of a particular Auction Algorithm
void run_auction(const Graph& graph, const int& verbose, const std::string& name, RunAuction& runauc)
{
	
    auto t = now();
    if (name == "auction_original")
    {
        std::cout << "Running " << name << "... ";
        runauc.auction.original_auction(graph, runauc.assignments);
    }  
    else
    {
        std::cout << "Running " << name << " with epsilon scaling factor: " << runauc.scaling_factor << "... ";
        runauc.auction.e_scaling_auction(graph, runauc.assignments, runauc.scaling_factor);
    }
    
    runauc.elapsed = now() - t;
	

    if (std::find(runauc.assignments.begin(), runauc.assignments.end(), -1) == runauc.assignments.end())
    {
        runauc.iterations = runauc.auction.getNIterationAu();
        runauc.cost = runauc.auction.getTotalCost(graph);
        std::cout << "Finished\n";
    }
    else
    {
        runauc.iterations = -1;
        runauc.cost = static_cast<Weight>(-1);
        std::cout << "Error\n";
    }
    
}


std::string perform_au(const Graph& graph, std::map<std::string, RunAuction>& auction_results, const int& verbose)
{

    int n = int(boost::num_vertices(graph) / 2);
    std::tuple<std::string, Weight, Duration> best ("none", 0, static_cast<Duration>(0)); // Temporal variable to store the best algorithm attributes

    
    for (auto& run : auction_results) // For every elelmtn in the map
    {
        if (run.first != "none")
        {
            run_auction(graph, verbose, run.first, run.second); // RUn the specific Auction Algorithm

            // If the performance was better than the previous one update the tuple
            if ((run.second.cost > std::get<1>(best)) || (run.second.cost == std::get<1>(best) && run.second.elapsed < std::get<2>(best)))
            {
                std::get<0>(best) = run.first;
                std::get<1>(best) = run.second.cost;
                std::get<2>(best) = run.second.elapsed;
            }
        }
    }
    
    std::string best_method_name = std::get<0>(best); // Get the name of the best Auction Algorithm
	
    if (auction_results.at(best_method_name).cost == static_cast<Weight>(-1)) // If the best algorithm could not solve the assignment problem update the 'none' elemnt of the map
    {
        std::cout << "No matching found\n";
        if (verbose) auction_results.at(best_method_name).auction.printProprieties();
        auction_results.at("none").iterations = -1;
        auction_results.at("none").elapsed = static_cast<Duration>(-1);
        auction_results.at("none").cost = static_cast<Weight>(-1);
        return std::string("none");
    }
    else
    {
        // In case the best algorithm solve the Assignment Problem

        std::cout << "The matching is: ";

        // Print the Bidder - Item matchs with the edge weight, in the following form: (FROM vertex, TO vertex, Verex WEIGHT)
        for (int bidder = 0; bidder < n; ++bidder)
            std::cout << "(" << bidder << "," << auction_results.at(best_method_name).assignments[bidder] << "," <<
				(boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder, auction_results.at(best_method_name).assignments[bidder] + n, graph)).first)) << ")";
        std::cout << "\n";

        if (verbose) auction_results.at(best_method_name).auction.printProprieties();

        return best_method_name; // Return the name of the best Auction Algorithm
    }
}
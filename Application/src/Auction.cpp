#include "../include/Auction.h"
#include "../include/BipartiteGraph.h"
#include "../include/RunAuction.h"



void run_auction(const Graph& graph, const int& verbose, const std::string& name, RunAuction& runauc)
{
	
    auto t = now();
    if (name == "naive_auction")
    {
        std::cout << "Running " << name << "... ";
        runauc.auction.naive_auction(graph, runauc.assignments);
    }  
    else
    {
        std::cout << "Running " << name << " with epsilon: " << runauc.scaling_factor << "... ";
        runauc.auction.e_scaling(graph, runauc.assignments, runauc.scaling_factor);
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
    std::tuple<std::string, Weight, Duration> best ("none", 0, static_cast<Duration>(0));
    bool solved = true;

    for (auto& run : auction_results)
    {
        if (run.first != "none") {
            run_auction(graph, verbose, run.first, run.second);

            if ((run.second.cost > std::get<1>(best)) || (run.second.cost == std::get<1>(best) && run.second.elapsed < std::get<2>(best)))
            {
                std::get<0>(best) = run.first;
                std::get<1>(best) = run.second.cost;
                std::get<2>(best) = run.second.elapsed;
            }
        }
    }
    
    std::string best_method_name = std::get<0>(best);
	
    if (std::find(auction_results.at(best_method_name).assignments.begin(), auction_results.at(best_method_name).assignments.end(), -1) != auction_results.at(best_method_name).assignments.end()) { solved = false; }

    if (!solved)
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
        /* (FROM vertex, TO vertex, Verex WEIGHT )*/
        std::cout << "The matching is: ";

        for (int bidder = 0; bidder < n; ++bidder)
            std::cout << "(" << bidder << "," << auction_results.at(best_method_name).assignments[bidder] << "," <<
				(boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder, auction_results.at(best_method_name).assignments[bidder] + n, graph)).first)) << ")"; //* 10'000.0 
        std::cout << "\n";

        if (verbose) auction_results.at(best_method_name).auction.printProprieties();

        //std::cout << "naive_auction:" << auction_results.at("naive_auction").auction.getTotalCost(graph) << "\te_scaling: " << auction_results.at("e_scaling").auction.getTotalCost(graph) << "\n";


        return best_method_name;

    }
}
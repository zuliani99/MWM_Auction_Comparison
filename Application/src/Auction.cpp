#include "../include/Auction.h"
#include "../include/BipartiteGraph.h"
#include "../include/RunAuction.h"



void run_auction(const Graph& graph, const int& verbose, const std::string& name, RunAuction& runauc)
{
	std::cout << "Running " << name << "... ";
	auto t = now();
	if(name == "naive_auction") runauc.auction.naive_auction(graph, runauc.assignments);
	else runauc.auction.e_scaling(graph, runauc.assignments);
    runauc.elapsed = now() - t;
	std::cout << "Finished\n";

    //if (verbose) runauc.auction.printProprieties();
    runauc.iterations = runauc.auction.getNIterationAu();
    runauc.cost = runauc.auction.getTotalCost(graph);
}


std::string perform_au(const Graph& graph, std::map<std::string, RunAuction>& auction_results, const int& verbose)
{

    int n = int(boost::num_vertices(graph) / 2);
    std::string best = "naive_auction";
    bool solved = true;

	std::cout << "\n";

    for (auto& run : auction_results)
        if (run.first != "none") { run_auction(graph, verbose, run.first, run.second); }


    if (auction_results.at("naive_auction").cost < auction_results.at("e_scaling").cost) 
	{ best = "e_scaling"; }
	else if (auction_results.at("naive_auction").cost == auction_results.at("e_scaling").cost)
	{
		if(auction_results.at("naive_auction").elapsed > auction_results.at("e_scaling").elapsed)
		{ best = "e_scaling"; }
	}
	

    if (std::find(auction_results.at(best).assignments.begin(), auction_results.at(best).assignments.end(), -1) != auction_results.at(best).assignments.end()) { solved = false; }

    if (!solved)
    {
        std::cout << "No matching found\n";
        if (verbose) auction_results.at(best).auction.printProprieties();
        auction_results.at("none").iterations = -1;
        auction_results.at("none").elapsed = static_cast<Duration>(-1);
        auction_results.at("none").cost = static_cast<Weight>(-1);
        //return std::make_pair<std::string, Weight>("none", static_cast<Weight>(-1));
        return std::string("none");
    }
    else
    {
        /* (FROM vertex, TO vertex, Verex WEIGHT )*/
        std::cout << "The matching is: ";

        for (int bidder = 0; bidder < n; ++bidder)
            std::cout << "(" << bidder << "," << auction_results.at(best).assignments[bidder] << "," << 
				(boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder, auction_results.at(best).assignments[bidder] + n, graph)).first)) << ")"; //* 10'000.0 
        std::cout << "\n";

        if (verbose) auction_results.at(best).auction.printProprieties();

        std::cout << "naive_auction:" << auction_results.at("naive_auction").auction.getTotalCost(graph) << "\te_scaling: " << auction_results.at("e_scaling").auction.getTotalCost(graph) << "\n";


        return best;

    }
}
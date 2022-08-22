#include "../include/Auction.h"
#include "../include/BipartiteGraph.h"
#include "../include/AuctionAlgorithm.hpp"


//std::pair<std::string, Weight>
std::pair<std::string, Weight> perform_au(const Graph& graph, std::vector<Weight>& c, std::vector<Duration>& el, std::vector<int>& it, Duration& elapsed, int& n_iteration_au, const int& verbose)
{
    int n = int(boost::num_vertices(graph) / 2);
    
    std::vector<std::string> name = { "naive_auction", "e_scaling_no_C", "e_scaling_with_C" };
	std::vector<std::vector<int>> assignments(3, std::vector(n, -1));
    
    Auction<Graph, Weight> auction1(n);
    auto t = now();
    auction1.naive_auction(graph, assignments[0]);
    el[0] = now() - t;
    if (verbose) auction1.printProprieties();
    it[0] = auction1.getNIterationAu();
    c[0] = auction1.getTotalCost(graph);

    Auction<Graph, Weight> auction2(n);
    t = now();
    auction2.e_scaling_no_C(graph, assignments[1]);
    el[1] = now() - t;
    if (verbose) auction2.printProprieties();
    it[1] = auction2.getNIterationAu();
    c[1] = auction2.getTotalCost(graph);

    Auction<Graph, Weight> auction3(n);
    t = now();
    auction3.e_scaling_with_C(graph, assignments[2]);
    el[2] = now() - t;
    if (verbose) auction3.printProprieties();
    it[2] = auction3.getNIterationAu();
    c[2] = auction3.getTotalCost(graph);

    auto max = std::max_element(c.begin(), c.end()) - c.begin();
    
    elapsed = el[max];
    n_iteration_au = it[max];
    return std::pair<std::string, Weight>(name[max], c[max]);



    //std::vector<int> assignments_1(n, -1);
    //std::vector<int> assignments_2(n, -1);

    //Auction<Graph, Weight> auction_problem_1(n/*, 10'000*/);
    //Auction<Graph, Weight> auction_problem_2(n/*, 10'000*/);

    /*auto t_start_1 = now();
    auction_problem_1.auction_algorithm_1(graph, assignments_1);
    elapsed = now() - t_start_1;

    auto t_start_2 = now();
    auction_problem_2.auction_algorithm_2(graph, assignments_2);
    auto elapsed_2 = now() - t_start_2;
	*/

	/*auto t_start = now();
    auction_problem.auction_algorithm(graph, assignments);
    elapsed = now() - t_start;*/

    //if (std::find(assignments.begin(), assignments.end(), -1) != assignments.end()) { solved = false; }
        
    /*if (!solved)
    {
        std::cout << " Finished \nNo matching found\n";
        if (verbose) auction_problem.printProprieties();
        n_iteration_au = -1;
        */
        //return std::make_pair<std::string, Weight>("none", static_cast<Weight>(-1));
		/*return static_cast<Weight>(-1);
    }
    else
    {*/
        /* (FROM vertex, TO vertex, Verex WEIGHT )*/
        /*std::cout << " Finished \nThe matching is: ";

        for (int bidder = 0; bidder < n; ++bidder)
            std::cout << "(" << bidder << "," << assignments[bidder] << "," << (boost::get(boost::edge_weight_t(), graph, (boost::edge(bidder, assignments[bidder] + n, graph)).first))  << ")"; //* 10'000.0 
        std::cout << "\n";

        if (verbose) auction_problem.printProprieties();
        n_iteration_au = auction_problem.getNIterationAu();
        */
        /*std::cout << auction_problem_1.getTotalCost(graph) << "     " << auction_problem_2.getTotalCost(graph) << "\n";

        if (auction_problem_1.getTotalCost(graph) > auction_problem_2.getTotalCost(graph))
        {
            n_iteration_au = auction_problem_1.getNIterationAu();
            return std::make_pair<std::string, Weight>("epsilon_scaling", auction_problem_1.getTotalCost(graph));
        }
            
        else
        {
            n_iteration_au = auction_problem_2.getNIterationAu();
            return std::make_pair<std::string, Weight>("standard_auction", auction_problem_2.getTotalCost(graph));
        }*/

		//return auction_problem.getTotalCost(graph);
        
    //}

    
}



/*



template <typename T>
struct Data
{
    T auction;
    std::vector<int> assignments;
    Weight cost = 0;
    int iterations = 0;
    Duration elapsed;
};

//std::pair<std::string, Weight>
std::pair<std::string, Weight> perform_au(const Graph& graph, std::vector<Weight>& c, std::vector<Duration>& el, std::vector<int>& it, Duration& elapsed, int& n_iteration_au, const int& verbose)
{
    int n = int(boost::num_vertices(graph) / 2);

    std::vector<std::string> name = { "naive_auction", "e_scaling_no_C", "e_scaling_with_C" };
    std::map<std::string, Data<Auction<Graph, Weight>>> auction_results;

    for (int i = 0; i < 3; ++i)
    {
        auction_results.insert(std::pair<std::string, Data<Auction<Graph, Weight>>>(name[i], Data{ Auction<Graph, Weight>(n), std::vector(n, -1)}));
    }


    //std::vector<std::vector<int>> assignments(3, std::vector(n, -1));

    auto t = now();
    auction_results["naive_auction"].auction.naive_auction(graph, auction_results["naive_auction"].assignments);
    auction_results["naive_auction"].elapsed = now() - t;
    if (verbose) auction_results[0].auction.printProprieties();
    auction_results["naive_auction"].iterations = auction_results["naive_auction"].auction.getNIterationAu();
    auction_results["naive_auction"].cost = auction_results["naive_auction"].auction.getTotalCost(graph);

    t = now();
    auction_results["e_scaling_no_C"].auction.e_scaling_no_C(graph, auction_results["e_scaling_no_C"].assignments);
    auction_results["e_scaling_no_C"].elapsed = now() - t;
    if (verbose) auction_results["e_scaling_no_C"].auction.printProprieties();
    auction_results["e_scaling_no_C"].iterations = auction_results["e_scaling_no_C"].auction.getNIterationAu();
    auction_results["e_scaling_no_C"].cost = auction_results["e_scaling_no_C"].auction.getTotalCost(graph);

    t = now();
    auction_results["e_scaling_with_C"].auction.e_scaling_with_C(graph, auction_results["e_scaling_with_C"].assignments);
    auction_results["e_scaling_with_C"].elapsed = now() - t;
    if (verbose) auction_results[0].auction.printProprieties();
    auction_results["e_scaling_with_C"].iterations = auction_results["e_scaling_with_C"].auction.getNIterationAu();
    auction_results["e_scaling_with_C"].cost = auction_results["e_scaling_with_C"].auction.getTotalCost(graph);

    std::string key_best;
    Weight max = 0;
    for (auto& res : auction_results)
    {
        if (res.second.cost > max)
        {
            max = res.second.cost;
            key_best = res.first;
        }
    }


    elapsed = auction_results[key_best].elapsed;
    n_iteration_au = auction_results[key_best].iterations;
    return std::pair<std::string, Weight>(key_best, auction_results[key_best].cost);


*/
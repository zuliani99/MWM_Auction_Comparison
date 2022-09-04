#ifndef _Utils_H
#define _Utils_H

#include <iostream>
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <ctime>
#include <sstream>


// Function that obtain the user inputs
void get_input(int& min, int& max, int& verbose, int& fully_connected)
{
	std::cout << "-------- MAXIMUM WEIGHTED MATCHING - AUCTION ALGORITHM BECHMARK --------\n\n";
	std::cout << "Do you want to activate VERBOSE mode? (0/1) ";
	std::cin >> verbose;
	std::cout << "On what type of bipartite graph would you like to work with, incomplete (0) or complete (1): ";
	std::cin >> fully_connected;
	std::cout << "Please specify the starting number of vertices per part: ";
	std::cin >> min;
	std::cout << "Please specify the ending number of vertices per part: ";
	std::cin >> max;

	if (min <= 0 || max <= 0 || min > max || (verbose != 0 && verbose != 1) || (fully_connected != 0 && fully_connected != 1))
		throw std::invalid_argument("Please insert correct input");

}


#endif

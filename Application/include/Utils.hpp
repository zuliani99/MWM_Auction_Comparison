#ifndef _Utils_H
#define _Utils_H

#include <iostream>
#include <stdexcept>
#include <numeric>

inline void check_empty_file()
{
	std::ifstream file;
	file.open("../data/results.csv");
	if (file) {
		std::ofstream ofs;
		ofs.open("../data/results.csv", std::ofstream::out | std::ofstream::trunc);
		ofs.close();
	}
}

void get_input(int& min, int& max, int& verbose, int& fully_connected)
{
	std::cout << "-------- MAXIMUM WEIGHTED MATCHING - AUCTION ALGORITHM BECHMARK --------\n\n";
	std::cout << "Do you want to active VERBOSE mode? (1/0) ";
	std::cin >> verbose;
	std::cout << "On what type of bipartite graph would you like to work with, compelte (1) or non complete (0): ";
	std::cin >> fully_connected;
	std::cout << "Please specify the starting number of vertices per part: ";
	std::cin >> min;
	std::cout << "Please specify the ending number of vertices per part: ";
	std::cin >> max;

	if (min > max || (verbose != 0 && verbose != 1) || (fully_connected != 0 && fully_connected != 1))
		throw std::invalid_argument("Please insert correct input");

}

#endif

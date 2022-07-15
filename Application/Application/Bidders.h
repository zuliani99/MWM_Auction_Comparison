#ifndef _Bidders_H
#define _Bidders_H

#include <vector>

class Bidder {
	public:
		double first_item;
		double second_item;
		int idx_first_item;
		Bidder() {
			first_item = -1.0;
			second_item = -1.0;
			idx_first_item = -1;
		}
	};

std::unordered_map<int, Bidder> retunr_n_bidders(int n) {
	std::unordered_map<int, Bidder> ret;
	for (int i = 0; i < n; ++i) {
		ret.insert({ i, Bidder() });
	}
	return ret;
}


#endif
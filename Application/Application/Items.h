#ifndef _Items_H
#define _Items_H

#include <vector>

class Item {
	public:
		double cost;
		double high_bid;
		int high_bidder;
		Item() {
			high_bid = -1.0;
			cost = 0.0;
			high_bidder = -1;
		}
	};

std::unordered_map<int, Item> retunr_n_items(int n) {
	std::unordered_map<int, Item> ret;
	for (int i = 0; i < n; ++i) {
		ret.insert({ i, Item() });
	}
	return ret;
}

#endif
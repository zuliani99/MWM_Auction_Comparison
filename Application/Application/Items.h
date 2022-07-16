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

#endif
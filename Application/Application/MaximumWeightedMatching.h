#ifndef _MWM_H
#define _MWM_H

#include "Graph.h"

Weight perform_mwm(Graph const& graph, duration& elapsed);


struct fmt {
    duration const& _d;

    friend std::ostream& operator<<(std::ostream& os, fmt f) {
        if (f._d >= 1min)      return os << (f._d / 1min) << " minutes " << (f._d % 1min) / 1s << " seconds";
        else if (f._d >= 1s)  return os << (f._d / 1.0s) << " seconds";
        else if (f._d >= 1ms) return os << (f._d / 1.0ms) << " milliseconds";
        else                  return os << (f._d / 1.0us) << " microseconds";
    }
};

#endif 
// pairsubs.hpp

#ifndef PAIRSUBS_H
#define PAIRSUBS_H

#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <utility>

struct IntervalVar {
    int start;
    int end;
    std::string text;
    std::string original;
    int nl;
};

struct MatchesVar {
    int start;
    int end;
    std::vector<IntervalVar> original;
    std::vector<IntervalVar> reference;
};

std::vector<MatchesVar> find_intersections_c(const std::vector<IntervalVar>& set_a, const std::vector<IntervalVar>& set_b);

#endif // PAIRSUBS_H

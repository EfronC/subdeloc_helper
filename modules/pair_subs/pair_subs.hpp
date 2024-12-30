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

// Define the IntervalSearch class
class IntervalSearch {
private:
    std::vector<IntervalVar> data;
    std::vector<int> start_times;

    void extract_starts() {
        for (const auto& interval : data) {
            start_times.push_back(interval.start);
        }
    }

public:
    IntervalSearch(const std::vector<IntervalVar>& intervals) : data(intervals) {
        extract_starts();
        std::sort(start_times.begin(), start_times.end());
    }

    std::vector<IntervalVar> find_overlapping_intervals(int start_q, int end_q);
};

#endif // PAIRSUBS_H

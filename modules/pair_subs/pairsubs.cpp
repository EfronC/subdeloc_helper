#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <utility>
#include <cstdlib>  // For std::getenv
#include "pair_subs.hpp"
using namespace std;

const int MARGIN = getenv("PAIR_MARGIN") ? stoi(getenv("PAIR_MARGIN")) : 1000;

// Check containment and margin conditions
pair<bool, bool> check_interval_conditions(const IntervalVar& a, const IntervalVar& b) {
    bool contained = (a.start >= b.start && a.end <= b.end) || (b.start >= a.start && b.end <= a.end);
    bool margin = (abs(a.start - b.start) <= MARGIN || abs(a.end - b.end) <= MARGIN);
    return {contained, margin};
}

// Find matches for an interval within a set
vector<IntervalVar> find_matches(const IntervalVar& interval, const vector<IntervalVar>& set) {
    vector<IntervalVar> matches;
    for (const auto& item : set) {
        // auto [contained, margin] = check_interval_conditions(interval, item);
        pair<bool, bool> conditions = check_interval_conditions(interval, item);
        bool contained = conditions.first;
        bool margin = conditions.second;
        if (contained || margin) matches.push_back(item);
        if (item.start >= interval.end && !margin) break;
    }
    return matches;
}

// Process interval with matches and determine grouped start/end
pair<MatchesVar, int> process_interval(const IntervalVar& interval, const vector<IntervalVar>& set, bool original) {
    vector<IntervalVar> matches = find_matches(interval, set);
    MatchesVar result;
    if (!matches.empty()) {
        result.start = min(interval.start, min_element(matches.begin(), matches.end(), 
                      [](const IntervalVar& a, const IntervalVar& b) { return a.start < b.start; })->start);
        result.end = max(interval.end, max_element(matches.begin(), matches.end(), 
                      [](const IntervalVar& a, const IntervalVar& b) { return a.end < b.end; })->end);
        (original ? result.original : result.reference).push_back(interval);
        (original ? result.reference : result.original) = matches;
    }
    return {result, static_cast<int>(matches.size())};
}

// Update matches in MatchesVar if they meet conditions
bool update_matches(MatchesVar& matches, const IntervalVar& interval, bool original) {
    if (!matches.original.empty() || !matches.reference.empty()) {
        auto& ref = original ? matches.reference : matches.original;
        // auto [contained, margin] = check_interval_conditions(interval, ref.back());
        pair<bool, bool> conditions = check_interval_conditions(interval, ref.back());
        bool contained = conditions.first;
        bool margin = conditions.second;
        if (contained || margin) (original ? matches.original : matches.reference).push_back(interval);
    }
    return true;
}

// Find intersections between two sets of intervals
vector<MatchesVar> find_intersections_c(const vector<IntervalVar>& set_a, const vector<IntervalVar>& set_b) {
    vector<MatchesVar> intersections;
    MatchesVar placeholder;
    size_t i = 0, j = 0;

    while (i < set_a.size() && j < set_b.size()) {
        const auto& a = set_a[i];
        const auto& b = set_b[j];
        // auto [contained, margin] = check_interval_conditions(a, b);
        pair<bool, bool> conditions = check_interval_conditions(a, b);
        bool contained = conditions.first;
        bool margin = conditions.second;

        if (contained || margin) {
            int carry;
            /*auto [matches, size] = (a.end - a.start >= b.end - b.start) ? process_interval(a, {set_b.begin() + j, set_b.end()}, true)
                                                                        : process_interval(b, {set_a.begin() + i, set_a.end()}, false);*/
            pair<MatchesVar, int> conditions = (a.end - a.start >= b.end - b.start) ? process_interval(a, {set_b.begin() + j, set_b.end()}, true)
                                                                        : process_interval(b, {set_a.begin() + i, set_a.end()}, false);
            MatchesVar matches = conditions.first;
            int size = conditions.second;
            
            carry = (a.end - a.start >= b.end - b.start) ? j += size : i += size;
            if (!matches.original.empty() || !matches.reference.empty()) intersections.push_back(matches);
            (a.end - a.start >= b.end - b.start) ? ++i : ++j;
        } else {
            (a.start <= b.start) ? update_matches(intersections.empty() ? placeholder : intersections.back(), a, true), ++i
                                 : update_matches(intersections.empty() ? placeholder : intersections.back(), b, false), ++j;
        }
    }

    while (i < set_a.size()) update_matches(intersections.empty() ? placeholder : intersections.back(), set_a[i++], true);
    while (j < set_b.size()) update_matches(intersections.empty() ? placeholder : intersections.back(), set_b[j++], false);

    return intersections;
}


// -------------------------------------- Fetch Interval -----------------------------------------------------------

// Implementation of the IntervalSearch class

std::vector<IntervalVar> IntervalSearch::find_overlapping_intervals(int start_q, int end_q) {
    const double MIN_INTERVAL_OVERLAP = 0.5;

    std::vector<IntervalVar> result;

    // Find the position where 'start_q - MARGIN' would fit
    auto it = std::lower_bound(start_times.begin(), start_times.end(), start_q - MARGIN);
    size_t idx = it - start_times.begin();

    for (size_t i = idx; i < data.size(); ++i) {
        const IntervalVar& obj = data[i];

        // Stop if the 'start' of the interval is beyond the margin
        if (obj.start > end_q + MARGIN) {
            break;
        }

        // Calculate overlap with the query range
        int overlap_start = std::max(obj.start, start_q - MARGIN);
        int overlap_end = std::min(obj.end, end_q + MARGIN);
        int overlap_length = std::max(0, overlap_end - overlap_start);

        // Calculate the interval's total length
        int interval_length = obj.end - obj.start;
        int range_length = end_q - start_q;

        // Check if at least 50% of the interval is inside the query range
        if (overlap_length >= MIN_INTERVAL_OVERLAP * interval_length || overlap_length >= MIN_INTERVAL_OVERLAP * range_length) {
            result.push_back(obj);
        }
    }

    return result;
}

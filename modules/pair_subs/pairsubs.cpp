#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <utility>
using namespace std;

struct IntervalVar {
    int start;
    int end;
    string text;
    string original;
    int nl;
};

struct MatchesVar {
    int start;
    int end;
    vector<IntervalVar> original;
    vector<IntervalVar> reference;
};

// Check containment and margin conditions
pair<bool, bool> check_interval_conditions(const IntervalVar& a, const IntervalVar& b) {
    const int MARGIN = 2000;
    bool contained = (a.start >= b.start && a.end <= b.end) || (b.start >= a.start && b.end <= a.end);
    bool margin = (abs(a.start - b.start) <= MARGIN || abs(a.end - b.end) <= MARGIN);
    return {contained, margin};
}

// Find matches for an interval within a set
vector<IntervalVar> find_matches(const IntervalVar& interval, const vector<IntervalVar>& set) {
    vector<IntervalVar> matches;
    for (const auto& item : set) {
        // auto [contained, margin] = check_interval_conditions(interval, item);
        std::pair<bool, bool> conditions = check_interval_conditions(interval, item);
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
        std::pair<bool, bool> conditions = check_interval_conditions(interval, ref.back());
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
        std::pair<bool, bool> conditions = check_interval_conditions(a, b);
        bool contained = conditions.first;
        bool margin = conditions.second;

        if (contained || margin) {
            int carry;
            /*auto [matches, size] = (a.end - a.start >= b.end - b.start) ? process_interval(a, {set_b.begin() + j, set_b.end()}, true)
                                                                        : process_interval(b, {set_a.begin() + i, set_a.end()}, false);*/
            std::pair<MatchesVar, int> conditions = (a.end - a.start >= b.end - b.start) ? process_interval(a, {set_b.begin() + j, set_b.end()}, true)
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

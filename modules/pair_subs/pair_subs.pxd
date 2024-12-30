from libcpp.vector cimport vector
from libcpp.string cimport string

cdef extern from "pair_subs.hpp":

    cdef struct IntervalVar:
        int start
        int end
        string text
        string original
        int nl

    cdef struct MatchesVar:
        int start
        int end
        vector[IntervalVar] original
        vector[IntervalVar] reference

    vector[MatchesVar] find_intersections_c(const vector[IntervalVar]& set_a, const vector[IntervalVar]& set_b)

    cdef cppclass IntervalSearch:
        IntervalSearch(const vector[IntervalVar]& intervals)
        vector[IntervalVar] find_overlapping_intervals(int start_q, int end_q)
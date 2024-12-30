from libcpp.vector cimport vector
from libcpp.string cimport string
from modules.pair_subs.pair_subs cimport IntervalVar, MatchesVar, find_intersections_c, IntervalSearch
from cython.operator cimport dereference

def interval_to_dict(IntervalVar interval):
    """
    Convert Example struct to a Python dictionary
    """
    return {
        "start": interval.start,
        "end": interval.end,
        "text": str(interval.text.decode('utf-8')),
        "original": str(interval.original.decode('utf-8')),
        "nl": interval.nl
    }

def match_to_dict(MatchesVar match):
    """
    Convert Example struct to a Python dictionary
    """
    return {
        "start": match.start,
        "end": match.end,
        "original": [interval_to_dict(s) for s in match.original],
        "reference": [interval_to_dict(s) for s in match.reference]
    }

def find_intersections(list set_a_dicts, list set_b_dicts):
    """
    Wrapper for the test function to work with Python dictionaries
    """
    # Create C++ vector of Example structs from Python list of dictionaries
    cdef vector[IntervalVar] set_a
    cdef IntervalVar interval
    for item in set_a_dicts:
        interval.start = item["start"]
        interval.end = item["end"]
        interval.text = item["text"].encode("utf-8")
        interval.original = item["original"].encode("utf-8")
        interval.nl = item["nl"]
        set_a.push_back(interval)

    cdef vector[IntervalVar] set_b
    for item in set_b_dicts:
        interval.start = item["start"]
        interval.end = item["end"]
        interval.text = item["text"].encode("utf-8")
        interval.original = item["original"].encode("utf-8")
        interval.nl = item["nl"]
        set_b.push_back(interval)

    # Call the C++ function
    cdef vector[MatchesVar] matches = find_intersections_c(set_a, set_b)
    
    # Convert result to list of Python dictionaries
    return [match_to_dict(match) for match in matches]

# ---------------------------------------------- Intervals ----------------------------------------------

cdef class PyIntervalSearch:
    cdef IntervalSearch* cpp_search

    def __cinit__(self, list intervals):
        """Initialize the C++ IntervalSearch class with a list of intervals."""
        cdef vector[IntervalVar] cpp_intervals
        cdef IntervalVar interval
        for item in intervals:
            interval.start = item["start"]
            interval.end = item["end"]
            interval.text = item["text"].encode("utf-8")
            interval.original = item["original"].encode("utf-8")
            interval.nl = item["nl"]
            cpp_intervals.push_back(interval)
        self.cpp_search = new IntervalSearch(cpp_intervals)

    def __dealloc__(self):
        del self.cpp_search

    cpdef list find_overlapping_intervals(self, int start_q, int end_q):
        """Wrapper for the find_overlapping_intervals method."""
        cdef vector[IntervalVar] cpp_result = self.cpp_search.find_overlapping_intervals(start_q, end_q)
        return [interval_to_dict(interval) for interval in cpp_result]
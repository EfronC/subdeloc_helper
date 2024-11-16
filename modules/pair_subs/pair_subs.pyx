from libcpp.vector cimport vector
from libcpp.string cimport string
from modules.pair_subs.pair_subs cimport IntervalVar, MatchesVar, find_intersections_c

def interval_to_dict(IntervalVar interval):
    """
    Convert Example struct to a Python dictionary
    """
    return {
        "start": interval.start,
        "end": interval.end,
        "text": str(interval.text.decode('utf-8')),
        "original": str(interval.text.decode('utf-8')),
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
        interval.nl = item["nl"]
        set_a.push_back(interval)

    cdef vector[IntervalVar] set_b
    for item in set_b_dicts:
        interval.start = item["start"]
        interval.end = item["end"]
        interval.text = item["text"].encode("utf-8")
        interval.nl = item["nl"]
        set_b.push_back(interval)

    # Call the C++ function
    cdef vector[MatchesVar] matches = find_intersections_c(set_a, set_b)
    
    # Convert result to list of Python dictionaries
    return [match_to_dict(match) for match in matches]

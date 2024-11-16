from libcpp.string cimport string
from libcpp.unordered_map cimport unordered_map
from libcpp.vector cimport vector

cdef extern from "modify_subs.hpp":
    string modifySubs(const string subfile, const string wordsfile)

cdef extern from "modify_subs.hpp":
    string overwriteSubs(const string subfile, const string wordsfile)

cdef extern from "modify_subs.hpp":
    string find_key_by_string(const unordered_map[string, unordered_map[string, unordered_map[string, vector[string]]]] dictionary,
                              const string target_string,
                              const string search_array)
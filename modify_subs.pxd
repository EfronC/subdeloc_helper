from libcpp.string cimport string

cdef extern from "modify_subs.hpp":
    string modifySubs(const string subfile, const string wordsfile)
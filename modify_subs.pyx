from modify_subs cimport modifySubs
from modify_subs cimport overwriteSubs
from modify_subs cimport overwriteSubs
from modify_subs cimport find_key_by_string
from libcpp.string cimport string
from libcpp.unordered_map cimport unordered_map
from libcpp.vector cimport vector

def modify_subs_py(subfile: str, replacementFile: str) -> str:
    cdef string result = modifySubs(subfile.encode(), replacementFile.encode())
    return result.decode()

def overwrite_subs(subfile: str, replacementFile: str) -> str:
    cdef string result = overwriteSubs(subfile.encode(), replacementFile.encode())
    return result.decode()

cdef unordered_map[string, unordered_map[string, unordered_map[string, vector[string]]]] to_cpp_dict(dict py_dict):
    cdef unordered_map[string, unordered_map[string, unordered_map[string, vector[string]]]] cpp_dict
    cdef unordered_map[string, unordered_map[string, vector[string]]] inner_cpp_dict
    cdef string cpp_key, cpp_nested_key, cpp_group_key
    cdef unordered_map[string, vector[string]] cpp_group
    cdef vector[string] cpp_vector
    cdef object py_key, py_value, nested_key, nested_value, item

    for py_key, py_value in py_dict.items(): # honorific - value
        cpp_key = py_key.encode('utf-8')
        inner_cpp_dict.clear()  # Reset the inner_cpp_dict for each new key
        for nested_key, nested_value in py_value.items(): # san - value
            cpp_nested_key = nested_key.encode('utf-8')
            cpp_vector.clear()  # Reset the cpp_vector for each new nested key
            for group_key, group_value in nested_value.items():
                cpp_group_key = group_key.encode('utf-8')
                cpp_vector.clear()  # Reset the cpp_vector for each new nested key
                for item in group_value:
                    cpp_vector.push_back(item.encode('utf-8'))
                cpp_group[cpp_group_key] = cpp_vector #cpp_vector.push_back(item.encode('utf-8'))
            inner_cpp_dict[cpp_nested_key] = cpp_group
        cpp_dict[cpp_key] = inner_cpp_dict

    return cpp_dict

def find_key_by_string_wrapper(dict py_dictionary, str target_string, str search_array) -> str:
    cdef unordered_map[string, unordered_map[string, unordered_map[string, vector[string]]]] cpp_dict = to_cpp_dict(py_dictionary)
    cdef string result = find_key_by_string(cpp_dict, target_string.encode('utf-8'), search_array.encode('utf-8'))
    return result.decode('utf-8')


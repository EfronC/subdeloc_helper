from modify_subs cimport modifySubs
from libcpp.string cimport string

def modify_subs_py(subfile: str, replacementFile: str) -> str:
    cdef string result = modifySubs(subfile.encode(), replacementFile.encode())
    return result.decode()


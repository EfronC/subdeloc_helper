// modify_subs.hpp

#ifndef MODIFY_SUBS_HPP
#define MODIFY_SUBS_HPP

#include <string>
#include <unordered_map>
#include <vector>

std::string modifySubs(const std::string& subfile, const std::string& replacementFile);
std::string overwriteSubs(const std::string& subfile, const std::string& replacementFile);
std::string find_key_by_string(const std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>>& dictionary,
                               const std::string& target_string,
                               const std::string& search_array);

#endif // MODIFY_SUBS_HPP

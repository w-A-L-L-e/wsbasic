#ifndef STRING_UTILS_H 
#define STRING_UTILS_H
#include <string>

void replace_all(std::string& str, const std::string& from, const std::string& to);
std::string basename(const std::string& str);

#endif

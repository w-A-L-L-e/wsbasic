#include "string_utils.h"


void replace_all(std::string& str, const std::string& from, const std::string& to){
  size_t index = 0;
  while (true) {
    // find substring
    index = str.find(from, index);
    if (index == std::string::npos) break;

    // relace it with target to
    str.replace(index, from.length(), to);

    // jump further to after replacement
    index += to.length(); 
  }
}

std::string basename(const std::string& str) {
  size_t index = str.find(".");
  return str.substr(0, index);
}



#ifndef UTILS_PARENTHESES_CHECKER_H_
#define UTILS_PARENTHESES_CHECKER_H_

#include <stack>
#include <string>

namespace salog {

class valid_parentheses {
 public:
  bool operator()(std::string s);
  char get_parentheses_pair(char c);
};

}  // namespace salog

#endif  // UTILS_PARENTHESES_CHECKER_H_

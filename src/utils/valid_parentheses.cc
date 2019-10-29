#include "utils/valid_parentheses.h"

bool salog::valid_parentheses::operator()(std::string s) {
  if (!s.size()) return true;
  std::stack<char> parentheses_stack;
  
  for (char c : s) {
    if (c == '(' || c == '[' || c == '{' || c == '<') {
      parentheses_stack.push(c);
    } else if (c == ')' || c == ']' || c == '}' || c == '>') {
      if (parentheses_stack.empty())
        return false;
      if (parentheses_stack.top() != get_parentheses_pair(c))
        return false;
      parentheses_stack.pop();       
    }
  }
  
  return parentheses_stack.empty();
}

char salog::valid_parentheses::get_parentheses_pair(char c) {
  switch (c) {
    case ')':
      return '(';
    case ']':
      return '[';
    case '}':
      return '{';
    case '>':
      return '<';
    default:
      return '\0';
  }
}
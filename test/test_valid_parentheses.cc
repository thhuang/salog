#include "gtest/gtest.h"
#include "utils/valid_parentheses.h"

// test empty string
TEST(ValidParenthesesTest, TestEmptyStringFunction) {
  EXPECT_TRUE(salog::valid_parentheses()(""));
}

// test round brackets
TEST(ValidParenthesesTest, TestRoundBracketsFunction) {
  EXPECT_TRUE(salog::valid_parentheses()("()"));
  EXPECT_TRUE(salog::valid_parentheses()("()()()"));
  EXPECT_TRUE(salog::valid_parentheses()("(()()()()(()))"));
  EXPECT_TRUE(salog::valid_parentheses()("()()()()()()(((())))(())()"));
}

// test box brackets
TEST(ValidParenthesesTest, TestBoxBracketsFunction) {
  EXPECT_TRUE(salog::valid_parentheses()("[]"));
  EXPECT_TRUE(salog::valid_parentheses()("[][][]"));
  EXPECT_TRUE(salog::valid_parentheses()("[[][][][][[]]]"));
  EXPECT_TRUE(salog::valid_parentheses()("[][][][][][][[[[]]]][[]][]"));
}

// test curly brackets
TEST(ValidParenthesesTest, TestCurlyBracketsFunction) {
  EXPECT_TRUE(salog::valid_parentheses()("{}"));
  EXPECT_TRUE(salog::valid_parentheses()("{}{}{}"));
  EXPECT_TRUE(salog::valid_parentheses()("{{}{}{}{}{{}}}"));
  EXPECT_TRUE(salog::valid_parentheses()("{}{}{}{}{}{}{{{{}}}}{{}}{}"));
}

// test angle brackets
TEST(ValidParenthesesTest, TestAngleBracketsFunction) {
  EXPECT_TRUE(salog::valid_parentheses()("<>"));
  EXPECT_TRUE(salog::valid_parentheses()("<><><>"));
  EXPECT_TRUE(salog::valid_parentheses()("<<><><><><<>>>"));
  EXPECT_TRUE(salog::valid_parentheses()("<><><><><><><<<<>>>><<>><>"));
}

// test valid string
TEST(ValidParenthesesTest, TestValidFunction) {
  EXPECT_TRUE(salog::valid_parentheses()("[]{}<>()"));
  EXPECT_TRUE(salog::valid_parentheses()(""));
  EXPECT_TRUE(salog::valid_parentheses()("<<{{{}()()[]}}>([[]])([{}]{})>"));
  EXPECT_TRUE(salog::valid_parentheses()("({(()(([[[]]<<()([])()<>{((()))}>>[[]]])((()))<>)<>)})"));
}

// test invalid string
TEST(ValidParenthesesTest, TestInvalidFunction) {
  EXPECT_FALSE(salog::valid_parentheses()("<(()[]()))>"));
  EXPECT_FALSE(salog::valid_parentheses()("{{{}}{(()}{<>()<><}}{}>)"));
  EXPECT_FALSE(salog::valid_parentheses()("("));
  EXPECT_FALSE(salog::valid_parentheses()("}"));
  EXPECT_FALSE(salog::valid_parentheses()("([)]"));
}
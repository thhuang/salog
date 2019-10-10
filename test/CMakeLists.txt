cmake_minimum_required(VERSION 3.10)

set(this tests)

set(sources
  test_timer.cc
  test_valid_parentheses.cc
  test_salog.cc
)

add_executable(${this} ${sources})
target_link_libraries(${this} PRIVATE
  gtest_main
  salog
  utils
)

add_test(
  NAME ${this}
  COMMAND ${this}
)
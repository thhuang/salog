#include "gtest/gtest.h"
#include "utils/timer.h"
#include <chrono>
#include <thread>
#include <unistd.h>

// test ringed function
TEST(TimerTest, TestRingedFunction) {
  salog::Timer timer{std::chrono::milliseconds(3)};
  timer.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(1));  // 1 ms
  EXPECT_FALSE(timer.ringed());
  std::this_thread::sleep_for(std::chrono::milliseconds(2));  // 2 ms
  EXPECT_TRUE(timer.ringed());
}

// test pause function
TEST(StopwatchTest, TestPauseFunction) {
  salog::Timer timer{std::chrono::milliseconds(3)};
  timer.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(1));  // 1 ms
  timer.pause();
  EXPECT_FALSE(timer.ringed());
  std::this_thread::sleep_for(std::chrono::milliseconds(3));  // 3 ms
  EXPECT_FALSE(timer.ringed());
}

// test resume function
TEST(StopwatchTest, TestResumeFunction) {
  salog::Timer timer{std::chrono::milliseconds(3)};
  timer.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(1));  // 1 ms
  timer.pause();
  EXPECT_FALSE(timer.ringed());
  std::this_thread::sleep_for(std::chrono::milliseconds(4));  // 4 ms
  EXPECT_FALSE(timer.ringed());
  timer.resume();
  std::this_thread::sleep_for(std::chrono::milliseconds(3));  // 2 ms
  EXPECT_TRUE(timer.ringed());
}

// test reset function
TEST(TimerTest, TestResetFunction) {
  salog::Timer timer{std::chrono::milliseconds(2)};
  timer.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(2));  // 2 ms
  EXPECT_TRUE(timer.ringed());
  timer.reset();
  std::this_thread::sleep_for(std::chrono::milliseconds(3));  // 3 ms
  EXPECT_FALSE(timer.ringed());
}

// test start after reset function
TEST(TimerTest, TestStartAfterResetFunction) {
  salog::Timer timer{std::chrono::milliseconds(3)};
  timer.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(3));  // 3 ms
  EXPECT_TRUE(timer.ringed());
  timer.reset();
  std::this_thread::sleep_for(std::chrono::milliseconds(5));  // 5 ms
  EXPECT_FALSE(timer.ringed());
  timer.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(1));  // 1 ms
  EXPECT_FALSE(timer.ringed());
  std::this_thread::sleep_for(std::chrono::milliseconds(2));  // 2 ms
  EXPECT_TRUE(timer.ringed());
}
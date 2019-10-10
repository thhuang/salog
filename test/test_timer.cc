#include "gtest/gtest.h"
#include "utils/timer.h"
#include <chrono>
#include <unistd.h>

// test ringed function
TEST(TimerTest, TestRingedFunction) {
  salog::Timer timer{std::chrono::milliseconds(3)};
  timer.start();
  EXPECT_FALSE(timer.ringed());
  usleep(3500);  // 3.5 ms
  EXPECT_TRUE(timer.ringed());
}

// test pause function
TEST(StopwatchTest, TestPauseFunction) {
  salog::Timer timer{std::chrono::milliseconds(3)};
  timer.start();
  timer.pause();
  EXPECT_FALSE(timer.ringed());
  usleep(3500);  // 3.5 ms
  EXPECT_FALSE(timer.ringed());
}

// test resume function
TEST(StopwatchTest, TestResumeFunction) {
  salog::Timer timer{std::chrono::milliseconds(3)};
  timer.start();
  timer.pause();
  EXPECT_FALSE(timer.ringed());
  usleep(3500);  // 3.5 ms
  EXPECT_FALSE(timer.ringed());
  timer.resume();
  usleep(3500);  // 3.5 ms
  EXPECT_TRUE(timer.ringed());
}

// test reset function
TEST(TimerTest, TestResetFunction) {
  salog::Timer timer{std::chrono::milliseconds(2)};
  timer.start();
  usleep(2500);  // 2.5 ms
  EXPECT_TRUE(timer.ringed());
  timer.reset();
  usleep(3000);  // 3 ms
  EXPECT_FALSE(timer.ringed());
}

// test start after reset function
TEST(TimerTest, TestStartAfterResetFunction) {
  salog::Timer timer{std::chrono::milliseconds(3)};
  timer.start();
  usleep(3500);  // 3.5 ms
  EXPECT_TRUE(timer.ringed());
  timer.reset();
  usleep(5000);  // 5 ms
  EXPECT_FALSE(timer.ringed());
  timer.start();
  EXPECT_FALSE(timer.ringed());
  usleep(3500);  // 3.5 ms
  EXPECT_TRUE(timer.ringed());
}
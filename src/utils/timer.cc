#include "utils/timer.h"

void salog::Timer::start() { 
  start_time_ = clock::now(); 
}

void salog::Timer::pause() {
  if (!paused_) {
    elapsed_ +=
        std::chrono::duration_cast<milliseconds>(clock::now() - start_time_);
    start_time_ = time_point{};
    paused_ = true;
  }
}

void salog::Timer::resume() {
  if (paused_) {
    start_time_ = clock::now();
    paused_ = false;
  }
}

void salog::Timer::reset() {
  start_time_ = time_point{};
  elapsed_ = milliseconds{0};
  paused_ = false;
}

salog::Timer::milliseconds salog::Timer::elapsed() {
  if (paused_)
    return elapsed_;
  return elapsed_ + std::chrono::duration_cast<milliseconds>(clock::now() - start_time_);
}

bool salog::Timer::ringed() {
  if (start_time_ == time_point{})
    return false;
  return elapsed() >= interval_;
}
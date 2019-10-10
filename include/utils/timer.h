#ifndef UTILS_TIMER_H_
#define UTILS_TIMER_H_

#include <chrono>

namespace salog {

class Timer {
  typedef std::chrono::high_resolution_clock clock;
  typedef clock::time_point time_point;
  typedef std::chrono::milliseconds milliseconds;
 
 public:
  Timer(milliseconds interval)
      : interval_{interval}, elapsed_{0} {};

  void start();
  void pause();
  void resume();
  void reset();
  bool ringed();

 private:
  time_point start_time_;
  milliseconds interval_;
  milliseconds elapsed_;
  bool paused_ = false;

  milliseconds elapsed();
};

}  // namespace salog

#endif  // UTILS_TIMER_H_

#ifndef SALOG_SALOG_H
#define SALOG_SALOG_H

#include <algorithm>
#include <condition_variable>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
namespace salog {

class SALog {
 protected:
  SALog() = default;
  SALog(const SALog&) = delete;
  SALog(SALog&&) = delete;
  SALog& operator=(const SALog&) = delete;
  SALog& operator=(SALog&&) = delete;

  static std::unique_ptr<SALog> logger;

 public:
  virtual ~SALog();

  template <typename T>
  SALog& operator<<(const T& value);
  SALog& operator<<(std::ios& (*pfunc)(std::ios&));
  SALog& operator<<(std::ios_base& (*pfunc)(std::ios_base&));
  SALog& operator<<(std::ostream& (*pfunc)(std::ostream&));

  virtual void set_file(std::string filename) {}

 protected:
  static SALog& get_logger() {
    if (logger == nullptr)
      logger = std::unique_ptr<SALog>(new SALog);
    return *logger;
  }

  class LogBuffer : public std::stringbuf {
   public:
    explicit LogBuffer(SALog& log);
    virtual ~LogBuffer() = default;

   protected:
    virtual int sync() override;

   private:
    SALog& log_;
  };

  class LogStream : public std::ostream {
   public:
    LogStream(SALog& log);
    virtual ~LogStream() = default;
    std::string get_string_and_clear();
    
   private:
    LogBuffer log_buffer_;
  };

  struct LogEntry {
    std::thread::id thread_id;
    std::function<void()> push_to_buffer;
  };

  std::mutex log_mutex_;
  std::condition_variable condition_variable_;

  std::list<LogEntry> log_queue_;
  std::list<LogEntry>::iterator current_log_ = log_queue_.end();
  std::thread::id _current_thread_id;

  bool closing_ = false;
  bool flush_ = false;

  LogStream log_stream_{*this};

  std::thread thread_{&SALog::serialize, this};

  void handle_sync();

  void serialize();

  virtual void flush();

  template <typename T> 
  SALog& write(T&& value);

  friend SALog& get_salog();
};

SALog& get_salog();

extern SALog& tlogger;

}  // namespace salog

template <typename T>
salog::SALog& salog::SALog::operator<<(const T& value) {
  return write(value);
}

template <typename T>
salog::SALog& salog::SALog::write(T&& value) {
  const auto id = std::this_thread::get_id();

  std::lock_guard<std::mutex> lock{log_mutex_};
  log_queue_.emplace_back(LogEntry{id, [=, value = std::forward<T>(value)] {
    log_stream_ << value;
  }});

  // If the current thread id is unknown
  if (_current_thread_id == std::thread::id{}) {
    current_log_ = std::begin(log_queue_);
    _current_thread_id = current_log_->thread_id;
    condition_variable_.notify_one();
    
  } else if (_current_thread_id == id) {
    if (current_log_ == log_queue_.end())
      current_log_ = std::prev(log_queue_.end());

    condition_variable_.notify_one();
  }

  return *this;
}

#endif  // SALOG_SALOG_H
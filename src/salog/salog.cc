#include "salog/salog.h"

salog::SALog::~SALog() {
  {
    std::lock_guard<std::mutex> lock{log_mutex_};
    closing_ = true;
  }

  condition_variable_.notify_one();
  thread_.join();
}

salog::SALog& salog::SALog::operator<<(std::ios& (*pfunc)(std::ios&)) {
  return write(pfunc);
}

salog::SALog& salog::SALog::operator<<(std::ios_base& (*pfunc)(std::ios_base&)) {
  return write(pfunc);
}

salog::SALog& salog::SALog::operator<<(std::ostream& (*pfunc)(std::ostream&)) {
  return write(pfunc);
}


void salog::SALog::handle_sync() {
  flush_ = true;
}

void salog::SALog::flush() {
  std::cout << log_stream_.get_string_and_clear();
  flush_ = false;
}

void salog::SALog::serialize() {
  while (true) {
    std::unique_lock<std::mutex> lock{log_mutex_};
    condition_variable_.wait(lock, [=] { 
      return closing_ || current_log_ != log_queue_.end();
    });

    if (closing_) {
      // Push every log to the buffer
      while (!log_queue_.empty()) {
        if (current_log_ == log_queue_.end()) {
          current_log_ = std::begin(log_queue_);
          _current_thread_id = current_log_->thread_id;
        }  // if (current_log_ == log_queue_.end())

        while (current_log_ != log_queue_.end()) {
          if (_current_thread_id == current_log_->thread_id) {
            current_log_->push_to_buffer();
            current_log_ = log_queue_.erase(current_log_);
          } else {
            ++current_log_;
          }  
        } 
      }  // while (!log_queue_.empty())

      // Flush
      flush();
      break;
    }  // if (closing_)
      
    // Push every log in current thread to the buffer
    do {
      current_log_->push_to_buffer();

      if (flush_) {
        flush();

        log_queue_.erase(current_log_);
        current_log_ = std::begin(log_queue_);

        // Nothing in the queue
        if (current_log_ == log_queue_.end()) {
          _current_thread_id = {};
          break;
        }

        _current_thread_id = current_log_->thread_id;
      } else {
        current_log_ = std::find_if(log_queue_.erase(current_log_), log_queue_.end(), 
          [=](const LogEntry& entry) {
            return _current_thread_id == entry.thread_id;
          }
        );
      }  // if (flush_)
    } while (current_log_ != log_queue_.end() && _current_thread_id == current_log_->thread_id);
  }  // while (true)
}

salog::SALog& salog::get_salog() {
  return salog::SALog::get_logger();
}

salog::SALog& salog::get_file_salog() {
  return salog::FileSALog::get_flogger();
}

std::unique_ptr<salog::SALog> salog::SALog::logger = nullptr;
salog::SALog& salog::tlogger = salog::get_salog();

// TODO: refactor file_salog.cpp
std::unique_ptr<salog::SALog> salog::FileSALog::logger = nullptr;
salog::SALog& salog::flogger = salog::get_file_salog();
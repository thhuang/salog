#include "salog/file_salog.h"
#include "utils/timer.h"

void salog::FileSALog::set_file(std::string filename) {
  if (!fout_.is_open()) fout_.open(filename);
}

salog::FileSALog::~FileSALog() {
  fout_.close();
  
  salog::Timer timer{std::chrono::milliseconds(100)};  // 100 ms
  timer.start();
  while (!timer.ringed());
}

void salog::FileSALog::flush() {
  fout_ << log_stream_.get_string_and_clear();
  flush_ = false;
}

salog::SALog& salog::FileSALog::get_flogger() {
  if (logger == nullptr)
    logger = std::unique_ptr<SALog>(new FileSALog);
  return *logger;
}

salog::SALog& salog::get_file_salog() {
  return salog::FileSALog::get_flogger();
}

std::unique_ptr<salog::SALog> salog::FileSALog::logger = nullptr;
salog::SALog& salog::flogger = salog::get_file_salog();
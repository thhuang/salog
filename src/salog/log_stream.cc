#include "salog/salog.h"

salog::SALog::LogStream::LogStream(salog::SALog& log) 
      : std::ostream{&log_buffer_}, log_buffer_{log} {}

std::string salog::SALog::LogStream::get_string_and_clear() {
  const std::string log_string = log_buffer_.str();
  log_buffer_.str({});

  return log_string;
}
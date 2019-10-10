#include "salog/salog.h"

salog::SALog::LogBuffer::LogBuffer(salog::SALog& log)
  : std::stringbuf{}, log_{log} {}

int salog::SALog::LogBuffer::sync() {
  log_.handle_sync();
  return std::stringbuf::sync();
}
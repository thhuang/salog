#ifndef SALOG_FILE_SALOG_H
#define SALOG_FILE_SALOG_H

#include "salog/salog.h"

namespace salog {

class FileSALog : public SALog {
 public:
  void set_file(std::string filename) override;

  ~FileSALog();
  std::ofstream fout_{};

 private:
  static std::unique_ptr<SALog> logger;

  void flush() override;
  static SALog& get_flogger();

  friend SALog& get_file_salog();
};

SALog& get_file_salog();

extern SALog& flogger;

}

#endif  // SALOG_FILE_SALOG_H
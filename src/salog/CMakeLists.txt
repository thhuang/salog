add_library(salog SHARED
  salog.cc
  file_salog.cc
  log_buffer.cc
  log_stream.cc
)

target_link_libraries(salog salog_utils)
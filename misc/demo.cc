#include <iomanip>
#include <thread>
#include <salog/file_salog.h>
#include <salog/salog.h>

class BracketOutput {
 private:
  char left_bracket_;
  char right_bracket_;

 public:
  BracketOutput(char left_bracket, char right_bracket)
    : left_bracket_{left_bracket}, right_bracket_{right_bracket} {}
  void operator()(std::string text, int num_brackets, int num_line) {
    for (int i = 0; i < num_line; ++i) {
      for (int j = 0; j < num_brackets; ++j)
        salog::tlogger << left_bracket_;

      // BracketOutput to terminal
      salog::tlogger << text << ", thread index: ";
      salog::tlogger << std::setw(16) << std::left << std::setfill(' ')
                     << std::this_thread::get_id();
                    
      // BracketOutput to file
      salog::flogger << text << ", thread index: ";
      salog::flogger << std::setw(16) << std::left << std::setfill(' ')
                     << std::this_thread::get_id();

      for (int i = 0; i < num_brackets; ++i)
        salog::tlogger << right_bracket_;

      salog::tlogger << std::endl;
      salog::flogger << std::endl;
    }
  }
};

int main() {
  // Set output path
  salog::flogger.set_file("log.txt");

  int num_line = 10;
  int num_bracket = 30;

  std::thread t1(BracketOutput('(', ')'), "  Thread 1", num_bracket, num_line);
  std::thread t2(BracketOutput('[', ']'), "  Thread 2", num_bracket, num_line);
  std::thread t3(BracketOutput('{', '}'), "  Thread 3", num_bracket, num_line);
  BracketOutput('<', '>')("  Thread 0", num_bracket, num_line);

  t1.join();
  t2.join();
  t3.join();

  return 0;
}

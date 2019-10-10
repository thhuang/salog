#include <algorithm>
#include <cstdio>
#include <sstream>
#include <streambuf>
#include <string>
#include <thread>
#include "gtest/gtest.h"
#include "salog/salog.h"
#include "utils/timer.h"
#include "utils/valid_parentheses.h"

namespace {

class CoutRedirector {
 public:
  CoutRedirector(std::streambuf* new_buffer)
      : tmp_buffer(std::cout.rdbuf(new_buffer)) {}

  ~CoutRedirector() { std::cout.rdbuf(tmp_buffer); }

 private:
  std::streambuf* tmp_buffer;
};

class SALogTest : public ::testing::Test {
 protected:
  int repeat_test;
  std::chrono::milliseconds time_limit;
  std::stringstream target;
  std::stringstream buffer;
  std::string filename = "/tmp/log.txt";

  void SetUp() override { 
    time_limit = std::chrono::milliseconds(16); 
    repeat_test = 16;
    salog::flogger.set_file(filename);
  }
  void TearDown() override {
    buffer.str({});
    buffer.clear();
    std::remove(filename.c_str());
  };

 public:
  template <typename T>
  void output(T& out, std::string line = "I have something to say!") {
    out << "\n";
    out << "{\n";
    out << std::setw(30) << std::setfill('#') << '\n';
    out << line << '\n';
    out << std::setw(30) << std::setfill('#') << '\n';
    out << "This is a text output\n";
    out << std::setw(30) << std::setfill('#') << '\n';
    out << std::setprecision(5) << 3.1415926 << '\n';
    out << std::setw(30) << std::setfill('#') << '\n';
    out << std::showbase << std::hex;
    out << std::uppercase << 77 << '\n';
    out << std::setw(30) << std::setfill('#') << '\n';
    out << "}\n";
    out << std::endl;
  }
};

// test stream api with a time limit
TEST_F(SALogTest, TestStreamAPI) {
  for (int i = 0; i < repeat_test; ++i) {
    {
      CoutRedirector cout_redirector(buffer.rdbuf());

      output(salog::logger);
      output(target);

      salog::Timer timer{time_limit};
      timer.start();
      while (!timer.ringed());
    }
    EXPECT_EQ(buffer.str(), target.str())
      << "i = " << i << std::endl;
    target.str({});
    target.clear();
    buffer.str({});
    buffer.clear();
  }
}

// test parallel 1
TEST_F(SALogTest, TestParallelOne) {
  for (int i = 0; i < repeat_test; ++i) {
    {
      CoutRedirector cout_redirector(buffer.rdbuf());

      std::thread t1([&] {
        output(salog::logger, "Output 1 from thread 1");
        output(salog::logger, "Output 2 from thread 1");
        output(salog::logger, "Output 3 from thread 1");
      });

      output(salog::logger, "Output 1 from the main thread");
      output(salog::logger, "Output 2 from the main thread");
      output(salog::logger, "Output 3 from the main thread");

      std::thread t2([&] {
        output(salog::logger, "Output 1 from thread 2");
        output(salog::logger, "Output 2 from thread 2");
        output(salog::logger, "Output 3 from thread 2");
      });

      std::thread t3([&] {
        output(salog::logger, "Output 1 from thread 3");
        output(salog::logger, "Output 2 from thread 3");
        output(salog::logger, "Output 3 from thread 3");
      });

      t1.join();
      t2.join();
      t3.join();

      salog::Timer timer{time_limit};
      timer.start();
      while (!timer.ringed());
    }

    EXPECT_TRUE(salog::valid_parentheses()(buffer.str()))
      << "i = " << i << std::endl;
    buffer.str({});
    buffer.clear();
  }
}

// test parallel 2
TEST_F(SALogTest, TestParallelTwo) {
  for (int i = 0; i < repeat_test; ++i) {
    {
      CoutRedirector cout_redirector(buffer.rdbuf());

      for (int i = 0; i < 10; ++i) {
        salog::logger << "((((((((((((((((((((((((((((((((((((((((";
        salog::logger << "))))))))))))))))))))))))))))))))))))))))";
        salog::logger << std::endl;
      }

      std::thread t1([&] {
        for (int i = 0; i < 10; ++i) {
          salog::logger << "((((((((((((((((((((((((((((((((((((((((";
          salog::logger << "))))))))))))))))))))))))))))))))))))))))";
          salog::logger << std::endl;
        }
      });

      std::thread t2([&] {
        for (int i = 0; i < 10; ++i) {
          salog::logger << "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[";
          salog::logger << "]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]";
          salog::logger << std::endl;
        }
      });

      std::thread t3([&] {
        for (int i = 0; i < 10; ++i) {
          salog::logger << "{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{";
          salog::logger << "}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}";
          salog::logger << std::endl;
        }
      });

      t1.join();
      t2.join();
      t3.join();

      salog::Timer timer{time_limit};
      timer.start();
      while (!timer.ringed());
    }

    EXPECT_TRUE(salog::valid_parentheses()(buffer.str()))
      << "i = " << i << std::endl;
    buffer.str({});
    buffer.clear();
  }
}

// test parallel 3
TEST_F(SALogTest, TestParallelThree) {
  for (int i = 0; i < repeat_test; ++i) {
    {
      CoutRedirector cout_redirector(buffer.rdbuf());

      for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 100; ++j) salog::logger << "<";
        for (int j = 0; j < 100; ++j) salog::logger << ">";
        salog::logger << std::endl;
      }

      std::thread t1([&] {
        for (int i = 0; i < 10; ++i) {
          for (int j = 0; j < 100; ++j) salog::logger << "(";
          for (int j = 0; j < 100; ++j) salog::logger << ")";
          salog::logger << std::endl;
        }
      });

      std::thread t2([&] {
        for (int i = 0; i < 10; ++i) {
          for (int j = 0; j < 100; ++j) salog::logger << "[";
          for (int j = 0; j < 100; ++j) salog::logger << "]";
          salog::logger << std::endl;
        }
      });

      std::thread t3([&] {
        for (int i = 0; i < 10; ++i) {
          for (int j = 0; j < 100; ++j) salog::logger << "{";
          for (int j = 0; j < 100; ++j) salog::logger << "}";
          salog::logger << std::endl;
        }
      });

      t1.join();
      t2.join();
      t3.join();

      salog::Timer timer{time_limit};
      timer.start();
      while (!timer.ringed());
    }

    EXPECT_TRUE(salog::valid_parentheses()(buffer.str()))
      << "i = " << i << std::endl;
    buffer.str({});
    buffer.clear();
  }
}

// test file stream api with a time limit
 TEST_F(SALogTest, TestFileStreamAPI) {
   
   for (int i = 0; i < repeat_test; ++i) {
     {
      CoutRedirector cout_redirector(buffer.rdbuf());

      output(salog::flogger);
      output(salog::logger);
      output(target);

      salog::Timer timer{time_limit};
      timer.start();
      while (!timer.ringed());
     }
     EXPECT_EQ(buffer.str(), target.str())
       << "i = " << i << std::endl;
     target.str({});
     target.clear();
     buffer.str({});
     buffer.clear();
   }
 }

// test parallel with terminal and file output
TEST_F(SALogTest, TestParallelTerminalAndFile) {
  for (int i = 0; i < repeat_test; ++i) {
    {
      CoutRedirector cout_redirector(buffer.rdbuf());

      for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 100; ++j) salog::logger << "<";
        for (int j = 0; j < 100; ++j) salog::flogger << "<";
        for (int j = 0; j < 100; ++j) salog::logger << ">";
        for (int j = 0; j < 100; ++j) salog::flogger << ">";
        salog::logger << std::endl;
        salog::flogger << std::endl;
      }

      std::thread t1([&] {
        for (int i = 0; i < 10; ++i) {
          for (int j = 0; j < 100; ++j) salog::logger << "(";
          for (int j = 0; j < 100; ++j) salog::flogger << "(";
          for (int j = 0; j < 100; ++j) salog::logger << ")";
          for (int j = 0; j < 100; ++j) salog::flogger << ")";
          salog::logger << std::endl;
          salog::flogger << std::endl;
        }
      });

      std::thread t2([&] {
        for (int i = 0; i < 10; ++i) {
          for (int j = 0; j < 100; ++j) salog::logger << "[";
          for (int j = 0; j < 100; ++j) salog::flogger << "[";
          for (int j = 0; j < 100; ++j) salog::logger << "]";
          for (int j = 0; j < 100; ++j) salog::flogger << "]";
          salog::logger << std::endl;
          salog::flogger << std::endl;
        }
      });

      std::thread t3([&] {
        for (int i = 0; i < 10; ++i) {
          for (int j = 0; j < 100; ++j) salog::logger << "{";
          for (int j = 0; j < 100; ++j) salog::flogger << "{";
          for (int j = 0; j < 100; ++j) salog::logger << "}";
          for (int j = 0; j < 100; ++j) salog::flogger << "}";
          salog::logger << std::endl;
          salog::flogger << std::endl;
        }
      });

      t1.join();
      t2.join();
      t3.join();

      salog::Timer timer{time_limit};
      timer.start();
      while (!timer.ringed());
    }

    EXPECT_TRUE(salog::valid_parentheses()(buffer.str()))
      << "i = " << i << std::endl;
    buffer.str({});
    buffer.clear();

    std::ifstream fin(filename);
    std::string log_file{std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>()};
    EXPECT_TRUE(salog::valid_parentheses()(buffer.str()))
      << "i = " << i << std::endl;
  }
}

}  // namespace
#include <algorithm>
#include <sstream>
#include <streambuf>
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

  void SetUp() override { 
    time_limit = std::chrono::milliseconds(16); 
    repeat_test = 16;
  }
  void TearDown() override {
    buffer.str({});
    buffer.clear();
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
  salog::SALog log;

  for (int i = 0; i < repeat_test; ++i) {
    {
      CoutRedirector cout_redirector(buffer.rdbuf());

      output(log);
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
  salog::SALog log;

  for (int i = 0; i < repeat_test; ++i) {
    {
      CoutRedirector cout_redirector(buffer.rdbuf());

      std::thread t1([&] {
        output(log, "Output 1 from thread 1");
        output(log, "Output 2 from thread 1");
        output(log, "Output 3 from thread 1");
      });

      output(log, "Output 1 from the main thread");
      output(log, "Output 2 from the main thread");
      output(log, "Output 3 from the main thread");

      std::thread t2([&] {
        output(log, "Output 1 from thread 2");
        output(log, "Output 2 from thread 2");
        output(log, "Output 3 from thread 2");
      });

      std::thread t3([&] {
        output(log, "Output 1 from thread 3");
        output(log, "Output 2 from thread 3");
        output(log, "Output 3 from thread 3");
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
  salog::SALog log;

  for (int i = 0; i < repeat_test; ++i) {
    {
      CoutRedirector cout_redirector(buffer.rdbuf());

      for (int i = 0; i < 10; ++i) {
        log << "((((((((((((((((((((((((((((((((((((((((";
        log << "))))))))))))))))))))))))))))))))))))))))";
        log << std::endl;
      }

      std::thread t1([&] {
        for (int i = 0; i < 10; ++i) {
          log << "((((((((((((((((((((((((((((((((((((((((";
          log << "))))))))))))))))))))))))))))))))))))))))";
          log << std::endl;
        }
      });

      std::thread t2([&] {
        for (int i = 0; i < 10; ++i) {
          log << "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[";
          log << "]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]";
          log << std::endl;
        }
      });

      std::thread t3([&] {
        for (int i = 0; i < 10; ++i) {
          log << "{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{";
          log << "}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}";
          log << std::endl;
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
  salog::SALog log;

  for (int i = 0; i < repeat_test; ++i) {
    {
      CoutRedirector cout_redirector(buffer.rdbuf());

      for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 100; ++j) log << "<";
        for (int j = 0; j < 100; ++j) log << ">";
        log << std::endl;
      }

      std::thread t1([&] {
        for (int i = 0; i < 10; ++i) {
          for (int j = 0; j < 100; ++j) log << "(";
          for (int j = 0; j < 100; ++j) log << ")";
          log << std::endl;
        }
      });

      std::thread t2([&] {
        for (int i = 0; i < 10; ++i) {
          for (int j = 0; j < 100; ++j) log << "[";
          for (int j = 0; j < 100; ++j) log << "]";
          log << std::endl;
        }
      });

      std::thread t3([&] {
        for (int i = 0; i < 10; ++i) {
          for (int j = 0; j < 100; ++j) log << "{";
          for (int j = 0; j < 100; ++j) log << "}";
          log << std::endl;
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

}  // namespace
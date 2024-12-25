#ifndef MYLOGGER_H
#define MYLOGGER_H

#include <iostream>
#include <fstream>
#include <mutex>
#include <memory>
#include <queue>
#include <sstream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <time.h>
#include <format>
#include <condition_variable>
#include <filesystem>

namespace MyLogger{

enum class LogLevel {
  ERROR, 
  WARNING,
  INFO,
  DEBUG
};

class LogEntry{
public:
  std::stringstream content;

//TODO: Lots of
};

class Logger {
public:
  Logger(std::string program_name, std::string dir, int max_size);

  ~Logger();

  void Log(const std::string& content);


private:
  void writerFunc();

  void ensureDir(std::string dir);

  std::queue<std::string> _buffer;
  std::string _dir;
  long long _max_size;
  std::thread _writer;
  bool _exit;
  std::mutex _mtx;
  std::ofstream _file_stream;
  std::string _program_name;
  std::condition_variable _cv;
};

}

// helpers

#endif

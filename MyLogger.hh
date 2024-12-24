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
};

class Logger {
public:
  Logger(std::string program_name, std::string dir, int max_size): _dir(dir), _program_name(program_name) { //max size in MB
    _exit = false;
    _max_size = max_size*1048576;
    _writer = std::thread(&Logger::writerFunc, this);
  }

  ~Logger();


private:
  void writerFunc();

  std::queue<std::string> _buffer;
  std::string _dir;
  long long _max_size;
  std::thread _writer;
  bool _exit;
  std::mutex _mtx;
  std::ofstream _file_stream;
  std::string _program_name;
};

}

#endif

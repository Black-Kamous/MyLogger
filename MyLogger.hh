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

template <class C>
struct has_ostream_operator_impl {
  template <class V>    // 若C存在operator<<，则能匹配到这个模板，declval创建一个指定类型的占位符而不实际地进行实例化
                        // 下面这个decltype的返回值应当是cout << V的返回值，即std::ostream&
  static auto test(V*) -> decltype(std::declval<std::ostream&>() << std::declval<V>());  //这里的&是关键的

  template <typename>   // 否则匹配到这个模板，返回false_type
  static auto test(...) -> std::false_type;

  // type记录了匹配到哪个模板，返回结果不同使得is_same结果不同
  using type = typename std::is_same<std::ostream&, decltype(test<C>(0))>::type;
};


template <class C>    //struct继承了上面的type，它的value只会是true/false
struct has_ostream_operator : has_ostream_operator_impl<C>::type {};

namespace MyLogger{

enum class LogLevel {
  ERROR, 
  WARNING,
  INFO,
  DEBUG
};

class LogEntry{
public:
  LogEntry() = default;
  LogEntry(LogLevel ll, 
          const char* file,
          const char* func,
          uint32_t line);

  ~LogEntry() {this->produce(); std::cout << "LogEntry deleted" << std::endl;}

  // enable_if 作为模板参数，如果结果为false_type则无法完成实例化
  template <class V, typename = std::enable_if_t<has_ostream_operator<V>::value>>
  std::ostream& operator<<(V item){
    return (content << item << " ");
  };

  // 当上面模板无法实例化时，激活下面的模板函数
  template <class V>
  typename std::enable_if<!has_ostream_operator<V>::value, std::ostream&>::type operator<<(V item) {
      //static_assert(has_ostream_operator<V>::value, "Type does not have an ostream operator<< defined.");
      return content << "[Not Stringifiable]" << " ";
  }
  
  void produce();

  std::stringstream content;
private:
  LogLevel _ll;
  std::string _file;
  std::string _func;
  uint32_t _line;

//TODO: Lots of
};

#define __LOG(level, file, func, line) LogEntry(level, file, func, line)

#define LOG(level) __LOG(level, __FILE__, __FUNCTION__, __LINE__)

class Logger {
public:
  Logger(const Logger&) = delete;
  Logger & operator = (const Logger &) = delete;

  void Log(const std::string& content);

  static Logger& getInstance();

  void setDir(std::string dir);

  void setMaxSize(int max_size);

  void setProgramName(std::string prog_name);

private:
  Logger();

  ~Logger();

  void writerFunc();

  void ensureDir(std::string dir);

  std::queue<std::string> _buffer;
  std::string _dir = "/var/log";
  long long _max_size = 4*1048576;
  std::thread _writer;
  bool _exit;
  std::mutex _mtx;
  std::ofstream _file_stream;
  std::string _program_name = "Logger";
  std::condition_variable _cv;
};

}

// helpers

#endif

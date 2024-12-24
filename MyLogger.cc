#include "MyLogger.hh"

namespace MyLogger {

Logger::~Logger(){
  _exit = true;
  _writer.join();
};

void Logger::writerFunc(){
  int bytes = 0;
  while(true){
    std::unique_lock<std::mutex> lck(_mtx);
    if(!_file_stream.is_open()){
      auto now = std::chrono::system_clock::now();
      auto date = std::chrono::system_clock::to_time_t(now);
      struct tm tm;
      localtime_r(&date, &tm);
      std::string timestr = std::format("{}_{}_{}_{}_{}", tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
      std::string file_name = std::format("{}-LOG-{}.log", _program_name, timestr);
      file_name = _dir + file_name;
      _file_stream.open(file_name, std::ios::out | std::ios::app);
    }
    while(!_buffer.empty()){
      auto loge = _buffer.front();
      _buffer.pop();
      _file_stream << loge << std::endl;
      bytes += loge.size()+1;
      if(bytes >= _max_size){
        bytes = 0;
        _file_stream.close();
        break;
      }
    }
  }
};


}
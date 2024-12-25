#include "MyLogger.hh"

namespace MyLogger {

Logger::Logger(std::string program_name, std::string dir, int max_size): _dir(dir), _program_name(program_name) { //max size in MB
    _exit = false;
    _max_size = max_size*1048576;
    ensureDir(_dir);
    std::cout << "starting writer thread" << std::endl;
    _writer = std::thread(&Logger::writerFunc, this);
};

Logger::~Logger(){
  _exit = true;
  std::cout << "Logger deleted" << std::endl;
  _cv.notify_all();
  _writer.join();
  _file_stream.close();
};

void Logger::Log(const std::string& content) {
  std::unique_lock<std::mutex> lck(_mtx);
  _buffer.push(content);
  _cv.notify_all();
};

void Logger::writerFunc(){
  int bytes = 0;
  std::cout << "writer thread started" << std::endl;
  while(true){
    std::unique_lock<std::mutex> lck(_mtx);
    _cv.wait(lck, [this](){return (!_buffer.empty() || _exit);});
    if(!_file_stream.is_open()){
      auto now = std::chrono::system_clock::now();
      auto date = std::chrono::system_clock::to_time_t(now);
      struct tm tm;
      localtime_r(&date, &tm);
      std::string timestr = std::format("{:02}_{:02}_{:02}_{:02}_{:02}", tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
      std::string file_name = std::format("{}-LOG-{}.log", _program_name, timestr);
      file_name = _dir + file_name;
      std::cout << file_name << std::endl;
      _file_stream.open(file_name, std::ios::out | std::ios::app);
      if(!_file_stream.is_open()){std::cout << "Cannot open log file" << std::endl; break;}
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
    if(_exit && _buffer.empty()) break;
  }
};

void Logger::ensureDir(std::string dir) {
  std::filesystem::path dirpath(dir);
  if(!std::filesystem::exists(dirpath)){
    std::filesystem::create_directories(dirpath);
    return;
  }
  std::filesystem::directory_entry entry(dirpath);
  if(entry.status().type() != std::filesystem::file_type::directory){
    std::filesystem::remove(dirpath);
    std::filesystem::create_directory(dirpath);
  }
}

} // namespace MyLogger
#include "MyLogger.hh"

namespace MyLogger {

Logger::Logger() { //max size in MB
    _exit = false;
    _writer = std::thread(&Logger::writerFunc, this);
    std::cout << "Logger Inited" << std::endl;
};

Logger::~Logger(){
  _exit = true;
  _cv.notify_all();
  _writer.join();
  _file_stream.close();
  std::cout << "Logger deleted" << std::endl;
};

void Logger::Log(const std::string& content) {
  std::unique_lock<std::mutex> lck(_mtx);
  _buffer.push(content);
  _cv.notify_all();
};

Logger &Logger::getInstance() {
  static Logger instance;
  return instance;
};

void Logger::writerFunc(){
  int bytes = 0;
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
};

void Logger::setDir(std::string dir) {
  _dir = dir;
  ensureDir(_dir);
};

void Logger::setMaxSize(int max_size) {
  _max_size = max_size*1048576;
};

void Logger::setProgramName(std::string prog_name) {
  _program_name = prog_name;
};

void LogEntry::produce() {
  Logger &logger = Logger::getInstance();
  auto now = std::chrono::system_clock::now();
  std::stringstream id;
  id << std::this_thread::get_id();
  std::string lev;
  switch(_ll){
    case LogLevel::ERROR:
      lev = "ERROR";
      break;
    case LogLevel::WARNING:
      lev = "WARNING";
      break;
    case LogLevel::INFO:
      lev = "INFO";
      break;
    case LogLevel::DEBUG:
      lev = "DEBUG";
      break;
  }
  std::string logl = std::format("[{}] [thread {}] [{}] {} at {}:{}:{}", now, id.str(), lev, content.str(), _file, _func, _line);
  logger.Log(logl);
};

LogEntry::LogEntry(LogLevel ll, const char *file, const char *func,
                   uint32_t line) {_ll = ll;_file = file; _func = func; _line = line;};

} // namespace MyLogger
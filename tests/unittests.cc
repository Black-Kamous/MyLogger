#include "unittests.hh"

using namespace MyLogger;

int count = 0;

void basic_test(){ 
  Logger& logger = Logger::getInstance();
  logger.Log("Some log");
}


void repeat_thread(int thrd_idx){
  Logger& logger = Logger::getInstance();
  for (int i=0;i<100000;++i){
    std::string logcont = std::format("[Thread {}] Log seq {}", thrd_idx, count);
    count ++;
    logger.Log(logcont);
}
}

// void blocking_test(){
//     Logger logger("test_main", "/root/MyLogger-cpp/tests/logs/", 2);
//     std::vector<std::thread> thrds;
//     int thrd_num = 4;
//     for (int i=0;i<thrd_num;++i){
//         thrds.push_back(std::thread([&logger](){
//             for (int i=0;i<100;++i){
//             std::string logcont = std::format("[Thread {}] Log seq {}", 0, i);
//             logger.Log(logcont);
//         }
//         }));
//     }

//     sleep(1);

//     for(int i=0;i<thrd_num;++i){
//         thrds[i].join();
//     }
// }

void threaded_test(){
  std::vector<std::thread> thrds;
  int thrd_num = 10;
  for (int i=0;i<thrd_num;++i){
    thrds.push_back(std::thread(repeat_thread, i));
  }

  for(int i=0;i<thrd_num;++i){
    thrds[i].join();
  }
}


void newfile_test(){
  Logger& logger = Logger::getInstance();
  for(int i=0;i<524289;++i){
    logger.Log("abcdefg12345678");
  }
}

void slow_logging(){
  Logger& logger = Logger::getInstance();
  for(int i=0;i<10;++i){
    std::stringstream s;
    auto now = std::chrono::system_clock::now();
    s << "Log at time " << now;
    logger.Log(s.str());
    sleep(10);
  }
}

class HasPrint {
public:
  HasPrint* x;
  friend std::ostream& operator<<(std::ostream& ,const HasPrint&);
};

std::ostream& operator<<(std::ostream& os,const HasPrint& h) {return os << "has print";};

class NoPrint {
  NoPrint* x;
  //friend std::ostream& operator<<(std::ostream& ,const NoPrint&) = delete;
};

void metap_test(){
  std::cout << std::boolalpha;
  std::cout << "int " << has_ostream_operator<int>::value << std::endl;
  std::cout << "char " << has_ostream_operator<char>::value << std::endl;
  std::cout << "string " << has_ostream_operator<std::string>::value << std::endl;
  std::cout << "NoPrint " << has_ostream_operator<struct NoPrint>::value << std::endl;
  std::cout << "HasPrint " << has_ostream_operator<struct HasPrint>::value << std::endl;

  LogEntry le;
  int i=1;
  le << i;
  std::cout << "int " << le.content.str() <<std::endl;
  char c='6';
  le << c;
  std::cout << "char " << le.content.str() <<std::endl;
  NoPrint n;
  le << n;
  std::cout << "NoPrint " << le.content.str() <<std::endl;
  HasPrint h;
  le << h;
  std::cout << "HasPrint " << le.content.str() <<std::endl;
}

int main(){
  Logger& logger = Logger::getInstance();
  logger.setDir("/root/MyLogger-cpp/tests/logs/");
  logger.setMaxSize(8);
  logger.setProgramName("test_main");

  // TIMING_CALL(basic_test, "basic test", std::chrono::nanoseconds);

  // TIMING_CALL(threaded_test, "multi-thread logging", std::chrono::milliseconds);

  // TIMING_CALL(newfile_test, "switching to new log file", std::chrono::seconds);

  // TIMING_CALL(slow_logging, "staying in same log file", std::chrono::seconds);

  TIMING_CALL(metap_test, "testing meta-programming code's correctness", std::chrono::seconds);
  
  return 0;
}
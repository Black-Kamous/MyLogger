#include "../MyLogger.hh"
#include <unistd.h>

using namespace MyLogger;

Logger logger("test_main", "/root/MyLogger-cpp/tests/logs/", 8);
int count = 0;

void basic_test(){ 
    logger.Log("Some log");
}


void repeat_thread(int thrd_idx){
    for (int i=0;i<5;++i){
        std::string logcont = std::format("[Thread {}] Log seq {}", thrd_idx, count);
        count ++;
        logger.Log(logcont);
    }
}

void blocking_test(){
    Logger logger("test_main", "/root/MyLogger-cpp/tests/logs/", 2);
    std::vector<std::thread> thrds;
    int thrd_num = 4;
    for (int i=0;i<thrd_num;++i){
        thrds.push_back(std::thread([&logger](){
            for (int i=0;i<100;++i){
            std::string logcont = std::format("[Thread {}] Log seq {}", 0, i);
            logger.Log(logcont);
        }
        }));
    }

    sleep(1);

    for(int i=0;i<thrd_num;++i){
        thrds[i].join();
    }
}

void threaded_test(){
    std::vector<std::thread> thrds;
    int thrd_num = 4;
    for (int i=0;i<thrd_num;++i){
        thrds.push_back(std::thread(repeat_thread, i));
    }

    for(int i=0;i<thrd_num;++i){
        thrds[i].join();
    }
}


void newfile_test(){
    for(int i=0;i<524289;++i){
        logger.Log("abcdefg12345678");
    }
}


int main(){
    auto start = std::chrono::system_clock::now();
    newfile_test();
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    std::cout << "test duration " << duration << std::endl;
    
    return 0;
}
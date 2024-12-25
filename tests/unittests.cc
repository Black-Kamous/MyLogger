#include "../MyLogger.hh"

using namespace MyLogger;

int basic_test(){
    Logger logger("test_main", "/root/MyLogger-cpp/tests/logs/", 2);
    logger.Log("Some log");
    return 0;
}


int main(){
    basic_test();
    return 0;
}
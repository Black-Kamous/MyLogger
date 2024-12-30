#include "../MyLogger.hh"
#include <unistd.h>

#define TIMING_CALL(func, desc, precision)                                    \
    do{                                                                 \
    auto start = std::chrono::system_clock::now();                      \
    func();                                                             \
    auto end = std::chrono::system_clock::now();                        \
    auto duration = std::chrono::duration_cast<precision>(end-start);   \
    std::cout << "Unittest for " << desc << " executed, taking " << duration << std::endl;       \
    } while(0)


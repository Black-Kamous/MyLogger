CC 			= g++-13
LD			= g++-13
CXXFLAGS 	= -O0 -Wall -std=c++20 -fPIC -shared
LDFLAGS		= -lstdc++ -fPIC -shared -pthread
TEST_LDFLAGS= -lstdc++

SOURCES 	= $(wildcard *.cc)
TEST_SOURCES= $(wildcard tests/*.cc)

OBJS 		= $(patsubst %.cc, %.o, $(SOURCES))
TEST_OBJS 	= $(patsubst %.cc, %.o, $(TEST_SOURCES))

TARGET		= mylogger.so
TEST_TARGET = main

%.o : %.cc
	$(CC) $(CXXFLAGS) -c $< -o $@

tests/%.o : tests/%.cc
	$(CC) $(CXXFLAGS) -c $< -o $@

$(TARGET) : $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET)

$(TEST_TARGET) : $(TEST_OBJS)
	$(LD) $(TEST_LDFLAGS) $(TEST_OBJS) -o $(TEST_TARGET)

tests : $(TEST_OBJS)

all : $(TARGET) tests

clean : 
	rm -rf $(OBJS) $(TEST_OBJS) $(TARGET)

.PHONY : all tests clean


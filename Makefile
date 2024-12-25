CC 			= g++-13
LD			= g++-13
CXXFLAGS 	= -O0 -Wall -std=c++20 -fPIC -shared
LDFLAGS		= -lstdc++ -fPIC -shared -pthread
TEST_LDFLAGS= -lstdc++ -L.
TEST_LDLIBS = -lmylogger

SOURCES 	= $(wildcard *.cc)
TEST_SOURCES= $(wildcard tests/*.cc)

OBJS 		= $(patsubst %.cc, %.o, $(SOURCES))
TEST_OBJS 	= $(patsubst %.cc, %.o, $(TEST_SOURCES))

TARGET		:= libmylogger.so
TEST_TARGET := tests/main

all : $(TARGET) $(TEST_TARGET)

%.o : %.cc
	$(CC) $(CXXFLAGS) -c $< -o $@

tests/%.o : tests/%.cc
	$(CC) $(CXXFLAGS) -c $< -o $@

$(TARGET) : $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET)

$(TEST_TARGET) : $(TEST_OBJS) $(TARGET)
	$(LD) $(TEST_LDFLAGS) $(TEST_OBJS) $(TEST_LDLIBS) -o $(TEST_TARGET)

test : $(TEST_TARGET)
	LD_LIBRARY_PATH=. $(TEST_TARGET)

clean : 
	rm -rf $(OBJS) $(TEST_OBJS) $(TARGET)

clear_logs :
	rm -rf tests/logs/*

.PHONY : all test clean


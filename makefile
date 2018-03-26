CXX = g++
CXXFLAGS = -std=c++11
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: p1 test clean

p1: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o p1

*.o: src/*.cpp src/*.h
	$(CXX) $(CXXFLAGS) -c src/*.cpp

test:
	sh tests.sh

clean:
	rm -f $(OBJECTS) p1

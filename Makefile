#COMPILER
CXX = g++

#COMPILER FLAGS
CXXFLAGS = -std=c++11 -Wall -lzmq -lpthread

all: server plf

server: server.cpp
	$(CXX) -o server server.cpp $(CXXFLAGS)

plf: plf.cpp
	$(CXX) -o plf plf.cpp $(CXXFLAGS)

clean:
	rm -rf server plf



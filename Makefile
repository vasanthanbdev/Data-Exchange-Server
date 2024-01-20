# #COMPILER
# CXX = g++

# #COMPILER FLAGS
# CXXFLAGS = -std=c++11 -Wall -lzmq

# #SOURCE FILES
# SRCS = server.cpp client.cpp

# #OBJECT FILES
# OBJS = $(SRCS:.cpp=.o)

# #EXECUTABLE FILES
# TARGET = server client
 

# #DEFAULT TARGET
# all: $(TARGET)
# .PHONY: all


# #RULES

# #RULES TO BUILD EXECUTABLES
# #$@- TARGET, $^- OBJS
# $(TARGET): $(OBJS)
# 	$(CXX) $(CXXFLAGS) -o $@ $^  -lzmq

# #RULES TO BUILD OJECT FILES
# #$<- SRCS, $@- OBJS
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# #RULES TO CLEAN
# clean:
# 	rm -rf $(OBJS) $(TARGET)


all: server client

server: server.cpp
	g++ -o server server.cpp -lzmq

client: client.cpp
	g++ -o client client.cpp -lzmq

clean:
	-rm server client



# compiler
CXX = g++

# compiler flags
CXXFLAGS = -std=c++11 -Wall

#source files
SRCS = plf.cpp

#Object files
OBJS = $(SRCS:.cpp=.o)

#Executable file
TARGET = plf

#default target
all: $(TARGET)

#Rule to build the executable file
# $@ - TARGET, $^ - OBJS (dependencies)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^  

#Rule to build object files
# $< - SRCS, $@ - OBJS 
$(OBJS): $(SRCS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

#clean the generated files
clean:
	rm -f $(OBJS) $(TARGET)

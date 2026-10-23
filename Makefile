# Makefile Project #1 CI4835
# Usage:
# make all - compiles the project
# make clean - removes all object files and the executable

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -Iinclude

PROGS = servidor cliente

all: $(PROGS)

%: src/%.cpp include/socket.hpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(PROGS)

.PHONY: all clean
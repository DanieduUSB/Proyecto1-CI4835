# Makefile Project #1 CI4835
# Usage:
# make all - compiles the project
# make clean - removes all object files and the executable

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

PROGS = server client

all: $(PROGS)

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(PROGS)

.PHONY: all clean
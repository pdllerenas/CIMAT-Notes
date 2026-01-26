CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra -Wshadow

# Current file (can be overridden)
SRC ?= main.cpp
BIN := $(SRC:.cpp=)

# Optional runtime args
ARGS ?=

.PHONY: run build clean

build:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN)

run: build
	./$(BIN) $(ARGS)

clean:
	rm -f $(BIN)

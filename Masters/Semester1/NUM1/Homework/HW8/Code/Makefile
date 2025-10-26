CC = g++
CXXFLAGS=-g -std=c++20 -Wall -pedantic
LDFLAGS =

LIB_DIR =
LIBS_SRCS = 

TARGETS = p1 p2 p3 p4 p5 p6

all: $(TARGETS)

$(TARGETS): %: %.cpp $(LIBS_SRCS)
	$(CC) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run-%: %
	./$* $(ARGS)

clean:
	rm -f $(TARGETS)

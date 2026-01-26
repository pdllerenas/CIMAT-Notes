CC = g++
CXXFLAGS=-g -std=c++20 -Wall -pedantic
LDFLAGS =

LIB_DIR =
LIBS_SRCS = 

TARGETS = p1 p2 p3 p3-b p3-d p4

all: $(TARGETS)

$(TARGETS): %: %.cpp $(LIBS_SRCS)
	$(CC) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run-%: %
	./$* $(ARGS)

clean:
	rm -f $(TARGETS)

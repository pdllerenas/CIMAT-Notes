CC = g++
CXXFLAGS=-g -std=c++20 -Wall -pedantic
LDFLAGS =

LIB_DIR =
LIBS_SRCS = 

TARGETS = p2 p3-a p3-b p3-c

all: $(TARGETS)

$(TARGETS): %: %.cpp $(LIBS_SRCS)
	$(CC) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run-%: %
	./$* $(ARGS)

clean:
	rm -f $(TARGETS)

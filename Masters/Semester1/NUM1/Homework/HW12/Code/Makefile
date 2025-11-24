CC = g++
CXXFLAGS=-g -std=c++20 -Wall -pedantic -DPARALLEL
LDFLAGS =

LIB_DIR =
LIBS_SRCS = 

TARGETS = p1 p3

all: $(TARGETS)

$(TARGETS): %: %.cpp $(LIBS_SRCS)
	$(CC) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run-%: %
	./$* $(ARGS)

clean:
	rm -f $(TARGETS)

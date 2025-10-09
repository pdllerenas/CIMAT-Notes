CC = gcc
CFLAGS = -Wall -pedantic -O4 
LDFLAGS = -lm

LINALG_DIR = ../../utils/linalg
ROOTS_DIR  = ../../utils/roots
LIBS_SRCS = $(wildcard $(LINALG_DIR)/*.c) $(wildcard $(ROOTS_DIR)/*.c)

TARGETS = p1 p2 p3 p4 p5 p6

all: $(TARGETS)

$(TARGETS): %: %.c $(LIBS_SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

run-%: %
	./$* $(ARGS)

clean:
	rm -f $(TARGETS)


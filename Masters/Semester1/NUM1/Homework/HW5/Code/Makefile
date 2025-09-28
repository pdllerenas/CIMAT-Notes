CC = gcc
CFLAGS = -g -Wall -lm

ARGS = 

LIB_DIR = ../../utils/linalg

LIBS_SRCS = $(addprefix $(LIB_DIR)/, eigenvalues.c matrix.c vector.c matrix_factorization.c matrix_io.c matrix_operations.c linear_system.c)

.PHONY: all clean run-p1 run-p2

all: p1 p2

p1: p1.c $(LIBS_SRCS)
	$(CC) $^ $(CFLAGS) -o p1
p2: p2.c $(LIBS_SRCS)
	$(CC) $^ $(CFLAGS) -o p2

run-p1: p1
	./p1 $(ARGS)

run-p2: p2
	./p2 $(ARGS)

clean:
	rm -f p1 p2


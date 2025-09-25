CC = gcc
CFLAGS = -Wall -lm

ARGS = 

LIB_DIR = ../../utils/linalg

LIBS_SRCS = $(addprefix $(LIB_DIR)/, eigenvalues.c matrix.c vector.c matrix_factorization.c matrix_io.c matrix_operations.c linear_system.c)

.PHONY: all clean run-p1 run-p2 run-p3 run-p4 run-p5 run-p6

all: p1 p3 p4 p5 p6

p1: p1.c $(LIBS_SRCS)
	$(CC) $^ $(CFLAGS) -o p1
p2: p2.c $(LIBS_SRCS)
	$(CC) $^ $(CFLAGS) -o p2
p3: p3.c $(LIBS_SRCS)
	$(CC) $^ $(CFLAGS) -o p3
p4: p4.c $(LIBS_SRCS)
	$(CC) $^ $(CFLAGS) -o p4
p5: p5.c $(LIBS_SRCS)
	$(CC) $^ $(CFLAGS) -o p5
p6: p6.c $(LIBS_SRCS)
	$(CC) $^ $(CFLAGS) -o p6

run-p1: p1
	./p1 $(ARGS)

run-p2: p2
	./p2 $(ARGS)

run-p3: p3
	./p3 $(ARGS)

run-p4: p4
	./p4 $(ARGS)

run-p5: p5
	./p5 $(ARGS)

run-p6: p6
	./p6 $(ARGS)

clean:
	rm -f p1 p2 p3 p4 p5 p6


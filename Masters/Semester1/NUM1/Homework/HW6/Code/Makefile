CC = gcc
CFLAGS = -Wall -pedantic
LDFLAGS = -lm 

LIB_DIR = ../../utils/linalg
LIBS_SRCS = $(addprefix $(LIB_DIR)/, eigenvalues.c matrix.c vector.c matrix_factorization.c matrix_io.c matrix_operations.c linear_system.c)

TARGETS = p1 p2 p3 p4 p5 p6

all: $(TARGETS)

$(TARGETS): %: %.c $(LIBS_SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

run-%: %
	./$* $(ARGS)

clean:
	rm -f $(TARGETS)


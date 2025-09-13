CC = gcc

TARGET = main

ARGS = 

SRCS = main.c p1.c p2.c p3.c vectors.c

all: $(TARGET)

$(TARGET): $(SRCS) 
	$(CC) $(SRCS) -o $(TARGET)

.PHONY: clean rebuild run

rebuild: clean all

run: all 
	./$(TARGET) $(ARGS)

gaussian: p5.c
	gcc p1.c p2.c vectors.c p5.c -o gaussian

run_gaussian: gaussian
	./gaussian $(ARGS)

clean:
	rm -f $(TARGET)


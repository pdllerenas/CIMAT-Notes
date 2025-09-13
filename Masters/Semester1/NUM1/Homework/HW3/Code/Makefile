CC = gcc

TARGET = main

ARGS = 

SRCS = main.c p1.c p2.c p3.c vectors.c

all: $(TARGET)

$(TARGET): $(SRCS) 
	$(CC) $(SRCS) -o $(TARGET)

run: all 
	./$(TARGET) $(ARGS)

clean:
	rm -f $(TARGET)


CC = gcc
CFLAGS = -Wall -pthread
TARGET = matrix_mul

all: $(TARGET)

$(TARGET): ThreadedMatrixMultiply.c
	$(CC) $(CFLAGS) -o $(TARGET) ThreadedMatrixMultiply.c

clean: 
	rm -f $(TARGET)



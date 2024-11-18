CC = gcc
TARGET = server
CFLAGS = -pthread
FORMAT_FILES = main.c request.c static.c stats.c calc.c

all: format $(TARGET)

format:
	clang-format -i $(FORMAT_FILES)

$(TARGET): main.c request.c static.c stats.c calc.c
	$(CC) $(CFLAGS) main.c request.c static.c stats.c calc.c -o $(TARGET)

clean:
	rm -f $(TARGET)
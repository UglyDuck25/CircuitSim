CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -g

TARGET = parser

.PHONY: all clean

all: $(TARGET)


parser: parser.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET)

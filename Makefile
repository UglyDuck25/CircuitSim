CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -Werror -g

TARGET = init
SRC = init.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET)

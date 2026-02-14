CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2 -Iinclude
DEBUG_FLAGS = -std=c11 -Wall -Wextra -g -Iinclude

TARGET = bin/elc
SRC_DIR = src
BUILD_DIR = build

SRC = $(shell find $(SRC_DIR) -name "*.c")
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS = $(DEBUG_FLAGS)
debug: clean all

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run: all
	./$(TARGET) examples/hello_world.el

.PHONY: all clean debug run


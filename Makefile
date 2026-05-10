TARGET = order-management-system

CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g -std=c11
TEST_FLAGS = -Itests

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
TEST_DIR = tests

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/test_%.o, $(TEST_SRCS))


all: prepare $(TARGET)

prepare:
	@mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

test: CFLAGS += $(TEST_FLAGS)
test: clean prepare $(OBJS) $(TEST_OBJS)
	$(CC) $(OBJS) $(TEST_OBJS) -o $(TARGET)
	./$(TARGET) --test

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean run prepare
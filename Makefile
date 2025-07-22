# Set compiler and flags
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Iinclude -g -O2 

# Folders
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = test
STATIC_LIB = lib_emul6502cpu

# Variables for storing source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Variables for storing source and object test files
TEST_SRC = $(TEST_DIR)/test_cpu6502.c
TEST_OBJ = $(BUILD_DIR)/test_cpu6502.o
TEST_EXE = test_runner

# Check flags.
CHECK_FLAGS = $(shell pkg-config --cflags check)
CHECK_LIBS = $(shell pkg-config --libs check)

# .PHONY forces the makefile to execute the
# command if there is a file in the project 
# whose name matches the name of the command,
# otherwise the makefile would consider that 
# we already have this target and would not 
# execute this command. For example, if we have
# a file named all, then make all will not work
# without .PHONE.

.PHONY: all clean test

# Default target is build a static library
all: $(STATIC_LIB)

# Building a static library
$(STATIC_LIB): $(OBJS) | $(BUILD_DIR)
	ar crs $@ $^

# Compile all files (.c -> .o)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create a BUILD_DIR folder if it does not exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Building and running tests
test: CFLAGS += $(CHECK_FLAGS)
test: LDFLAGS += $(CHECK_LIBS) -pthread
test: $(STATIC_LIB) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(TEST_OBJ) $(STATIC_LIB) $(LDFLAGS) -o $(TEST_EXE)
	./$(TEST_EXE)

# valgrind
test: CFLAGS += $(CHECK_FLAGS)
test: LDFLAGS += $(CHECK_LIBS) -pthread
test: $(STATIC_LIB) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(TEST_OBJ) $(STATIC_LIB) $(LDFLAGS) -o $(TEST_EXE)
	./$(TEST_EXE)
	valgrind --leak-check=full --track-origins=yes ./$(TEST_EXE)

# Compile tests
$(BUILD_DIR)/test_cpu6502.o: $(TEST_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(STATIC_LIB) $(TEST_EXE)

valgrind: test
	valgrind --leak-check=full --track-origins=yes ./$(TEST_EXE)

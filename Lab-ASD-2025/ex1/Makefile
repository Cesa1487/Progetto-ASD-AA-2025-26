# This Makefile builds two executables:
#   - bin/test_ex1   : Test program for merge_sort and quick_sort
#   - bin/main_ex1   : Main program that sorts records read from a CSV file
#
# Usage:
#   make        -> compiles both executables
#   make clean  -> removes all binaries
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
BIN = bin

SRC = src/sort.c src/record.c src/sort_records.c
TEST = test/test_ex1.c
MAIN = main/main_ex1.c
DEBUG_SRC = src/debug_view.c src/record.c

$(BIN):
	mkdir -p $(BIN)

all: $(BIN)/test_ex1 $(BIN)/main_ex1 $(BIN)/debug_view

$(BIN)/test_ex1: $(SRC) $(TEST)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/main_ex1: $(SRC) $(MAIN)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/debug_view: $(DEBUG_SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(BIN)/*
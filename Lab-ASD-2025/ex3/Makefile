CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -I../ex2/include

SRC = src/priority_queue.c ../ex2/src/hash_table.c
TEST = test/test_ex3.c
MAIN = main/main_ex3.c

BIN = bin

all: $(BIN)/test_ex3 $(BIN)/main_ex3

$(BIN)/test_ex3: $(SRC) $(TEST)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/main_ex3: $(SRC) $(MAIN)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(BIN)/*
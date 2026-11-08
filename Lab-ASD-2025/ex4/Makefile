CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/hash_table.c src/graph.c src/breadth_first_visit.c
TEST = test/test_ex4.c
MAIN = main/main_ex4.c

BIN = bin

all: $(BIN)/test_ex4 $(BIN)/main_ex4

$(BIN):
	mkdir -p $(BIN)

$(BIN)/test_ex4: $(SRC) $(TEST)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/main_ex4: $(SRC) $(MAIN)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(BIN)/*
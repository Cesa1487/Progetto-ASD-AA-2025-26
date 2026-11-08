CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/edit_distance.c src/edit_distance.c
TEST = test/test_ex2.c
MAIN = main/main_ex2.c

BIN = bin

all: $(BIN)/test_ex2 $(BIN)/main_ex2

$(BIN)/test_ex2: $(SRC) $(TEST)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/main_ex2: $(SRC) $(MAIN)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(BIN)/*
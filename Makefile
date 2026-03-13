CC=gcc
SRC=$(wildcard ./src/*.c)
CFLAGS=-O3 -Werror -Wall -Wextra -lm
DFLAGS=-O0 -g -Wall -Wextra -lm
BUILD=build
EXE=$(BUILD)/pratt-parsing
TEST=$(BUILD)/pratt-parsing-test
DEBUG=$(BUILD)/pratt-parsing-debug

ifeq ($(OS),Windows_NT)
    CFLAGS += -D__USE_MINGW_ANSI_STDIO
endif

all: $(BUILD) $(EXE) $(TEST) $(DEBUG)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $(EXE) $(SRC)

$(TEST): $(SRC)
	$(CC) -DTEST $(CFLAGS) -o $(TEST) $(SRC)

$(DEBUG): $(SRC)
	$(CC) $(DEFS) $(DFLAGS) -o $(DEBUG) $(SRC)

run: $(EXE)
	./build/pratt-parsing

debug: $(DEBUG)
	./buid/pratt-parsing-debug

test: $(TEST)
	./buid/pratt-parsing-test

$(BUILD):
	mkdir build

.PHONY: clean
clean:
	rm -rf build
	rm -f *.txt

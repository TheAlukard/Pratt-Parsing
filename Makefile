CC=gcc
SRC=$(wildcard ./src/*.c)
CFLAGS=-O3 -Werror -Wall -Wextra
DFLAGS=-O0 -g -Wall -Wextra
LFLAGS=-lm
BUILD=build
EXE=$(BUILD)/pratt-parsing
TEST=$(BUILD)/pratt-parsing-test
DEBUG=$(BUILD)/pratt-parsing-debug
DEFS=

ifeq ($(OS),Windows_NT)
    CFLAGS += -D__USE_MINGW_ANSI_STDIO
endif

all: $(BUILD) $(EXE) $(TEST) $(DEBUG)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $(EXE) $(SRC) $(LFLAGS)

$(TEST): $(SRC)
	$(CC) -DTEST $(CFLAGS) -o $(TEST) $(SRC) $(LFLAGS)

$(DEBUG): $(SRC)
	$(CC) $(DEFS) $(DFLAGS) -o $(DEBUG) $(SRC) $(LFLAGS)

run: $(EXE)
	./$(EXE)

debug: $(DEBUG)
	./$(DEBUG)

test: $(TEST)
	./$(TEST)

$(BUILD):
	mkdir -p $(BUILD)

.PHONY: clean
clean:
	rm -rf build
	rm -f *.txt

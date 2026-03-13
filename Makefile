CC=gcc
SRC=$(shell ls ./src/*.c)
CFLAGS=-O3 -Werror -Wall -Wextra -lm
DFLAGS=-O0 -g -Wall -Wextra -lm
BUILD=build
EXE=$(BUILD)/pratt-parsing
TEST=$(BUILD)/pratt-parsing-test
DEB=$(BUILD)/pratt-parsing-debug

ifeq ($(OS),Windows_NT)
    CFLAGS += -D__USE_MINGW_ANSI_STDIO
endif


all: $(BUILD) $(EXE) $(TEST) $(DEB)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $(EXE) $(SRC)

test: $(SRC)
	$(CC) -DTEST $(CFLAGS) -o $(TEST) $(SRC)

debug: $(SRC)
	$(CC) $(DEFS) $(DFLAGS) -o $(DEB) $(SRC)

run: $(EXE)
	./build/pratt-parsing

$(BUILD):
	mkdir build

.PHONY: clean
clean:
	rm -rf build
	rm -f *.txt

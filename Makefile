CC=gcc
SRC=$(shell ls ./src/*.c)
CFLAGS=-O3 -Werror -Wall -Wextra -lm
DFLAGS=-O0 -g -Wall -Wextra -lm
DEFS=

ifeq ($(OS),Windows_NT)
    CFLAGS += -D__USE_MINGW_ANSI_STDIO
endif

all: PrattParsing

PrattParsing: $(SRC) 
	$(CC) $(CFLAGS) -o PrattParsing $(SRC)

test: $(SRC) 
	$(CC) -DTEST $(CFLAGS) -o test $(SRC)

debug: $(SRC)
	$(CC) $(DEFS) $(DFLAGS) -o PrattParsing $(SRC)

run: PrattParsing
	./PrattParsing

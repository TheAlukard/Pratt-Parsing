CC=gcc
SRC=$(shell ls ./src/*.c)
CFLAGS=-O3 -Werror -Wall -Wextra -lm
DFLAGS=-O0 -g -Wall -Wextra -lm

ifeq ($(OS),Windows_NT)
    CFLAGS += -D__USE_MINGW_ANSI_STDIO
endif

all: PrattParsing

PrattParsing: $(SRC) 
	$(CC) $(CFLAGS) -o PrattParsing $(SRC)

debug: $(SRC)
	$(CC) $(DFLAGS) -o PrattParsing $(SRC)

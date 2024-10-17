SRC=$(shell ls ./src/*.c)
CFLAGS=-O3 -Werror -Wall -Wextra -pedantic

ifeq ($(OS),Windows_NT)
    CFLAGS += -D__USE_MINGW_ANSI_STDIO
endif

PrattParsing: $(SRC) 
	gcc $(CFLAGS) -o PrattParsing $(SRC)

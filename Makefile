SRC=$(shell ls ./src/*.c)

PrattParsing.exe: $(SRC) 
	gcc -O3 -Werror -Wall -Wextra -pedantic -o PrattParsing.exe $(SRC)

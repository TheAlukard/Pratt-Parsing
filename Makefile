SRC=$(shell ls ./src/*.c)

PrattParsing.exe: $(SRC) 
	gcc -O3 -o PrattParsing.exe $(SRC)

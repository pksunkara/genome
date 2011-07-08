BIN= ./bin/
OBJ= ./obj/
SRC= ./src/

prefix= /usr/bin
INSTALL= install
CC= gcc
CFLAGS= -Wall -pedantic -g

all: $(BIN)genome

$(BIN)genome: $(OBJ)lex.yy.o $(OBJ)y.tab.o $(SRC)genome.c
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)lex.yy.o: $(SRC)lex.yy.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJ)y.tab.o: $(SRC)y.tab.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(SRC)lex.yy.c: $(SRC)genome_lex.l
	lex $(SRC)genome_lex.l

$(SRC)y.tab.c: $(SRC)genome_parser.y
	yacc -d $(SRC)genome_parser.y

install:
  $(INSTALL) $(BIN)genome $(prefix)genome

clean:
	rm $(BIN)* $(OBJ)* $(SRC)y.tab.c $(SRC)lex.yy.c


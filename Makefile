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

$(OBJ)lex.yy.o: lex.yy.c y.tab.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)y.tab.o: y.tab.c
	$(CC) $(CFLAGS) -c $^ -o $@

lex.yy.c: $(SRC)genome_lex.l
	lex $^

y.tab.c: $(SRC)genome_parser.y
	yacc -d $^

install: 
	$(INSTALL) $(BIN)genome $(prefix)/genome

clean:
	rm $(BIN)* $(OBJ)* y.tab.* lex.yy.*

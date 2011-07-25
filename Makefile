prefix= /usr/bin
INSTALL= install
CC= gcc
CFLAGS= -Wall -pedantic -g
RM= rm -rf

all: genome

genome: lex.yy.o y.tab.o genome.c
	$(CC) $(CFLAGS) $^ -o $@

lex.yy.o: lex.yy.c y.tab.c
	$(CC) $(CFLAGS) -c $< -o $@

y.tab.o: y.tab.c
	$(CC) $(CFLAGS) -c $^ -o $@

lex.yy.c: genome_lex.l
	lex $^

y.tab.c: genome_parser.y
	yacc -d $^

install: 
	$(INSTALL) genome $(prefix)/genome

clean:
	$(RM) *.o
	$(RM) y.tab.* lex.yy.* genome

prefix= /usr/bin

RUBY= ruby

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

lex.yy.c: analyzer.l
	lex $^

y.tab.c: parser.y
	yacc -d $^

install: 
	$(INSTALL) genome $(prefix)/genome

check:
	$(RUBY) test/run_all.rb

clean:
	$(RM) *.o
	$(RM) y.tab.* lex.yy.* genome

prefix= /usr/bin

NODE= node
TEST_SCRIPT= test/test.js

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

install: genome
	$(INSTALL) genome $(prefix)/genome

test: $(TEST_SCRIPT) genome
	$(NODE) $<

clean:
	$(RM) *.o
	$(RM) y.tab.* lex.yy.* genome

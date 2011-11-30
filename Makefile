prefix= /usr/bin

RUBY= ruby
TEST_SCRIPT= test/run_all.rb

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
	$(RUBY) $<

clean:
	$(RM) *.o
	$(RM) y.tab.* lex.yy.* genome

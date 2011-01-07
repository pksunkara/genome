all: exe

exe: yacc lex genome.c
	gcc -g lex.yy.o y.tab.o genome.c -o genome

lex: genome_lex.l
	lex genome_lex.l
	gcc -g -c lex.yy.c

yacc: genome_parser.y
	yacc -d genome_parser.y
	gcc -g -c y.tab.c

clean:
	rm -f y.tab.* lex.yy.* genome *.out *.o

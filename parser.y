%{

	#include <stdio.h>
	#include <string.h>
	#include <ctype.h>
	#include <stdlib.h>
	
	extern int yylex(void);
	extern char* yytext;
	extern int yyleng;
	
	int lineNum=1;
	int colNum=1;
	
	int yywrap(void);
	void count(void);
	void yyerror(char* msg);

	extern void insert_instr(int, int);
	extern void clear_stack(void);
	extern void slide(int);
	extern void push_number_into_stack(int);
	extern void pop_number(void);
	extern void pop_numbers_from_stack(int, int);
	extern void dup_stack(void);
	extern void dup_stack_n(int, int);
	extern void copy_nth(int, int);
	extern void arith(int);
	extern void crement(int, int);
	extern void print_whole_stack(int);
	extern void print_stack_n(int, int, int);
	extern void reverse_whole(void);
	extern void reverse_stack_n(int, int);
	extern void move_top(void);
	extern void move(int, int);
	extern void read_top(void);
	extern void read_n(int, int);
%}

%token AAA // Duplicate the whole stack
%token AAT // Duplicate top n items on the stack
%token AAC // Duplicate top item on the stack
%token AAG // Duplicate bottom n items on the stack

%token ATA // Push number onto stack
%token ATT // Pop top n items from stack
%token ATC // Pop number from stack (Discarding)
%token ATG // Pop bottom n items from stack

%token ACA // Clear the whole stack
%token ACT // Copy top nth item on the stack
%token ACC // Slide n items keeping top item
%token ACG // Copy bottom nth item on the stack

%token AGA // Reverse the whole stack
%token AGT // Reverse the top n items
%token AGC // Reverse the top 2 items
%token AGG // Reverse the bottom n items

%token TAA // Addition 1+2 items and push the result
%token TAT // Subtraction 2-1 items and push the result
%token TAC // Multiplication 2*1 items and push the result
%token TAG // Division 2/1 items and push the result
%token TTA // Increment the top item by n (if nothing n=1)
%token TTT // Decrement the top item by n (if nothing n=1)

%token TTC // Start block
%token TTG // Stop block

%token TCA // Jump unconditionally to start of the block
%token TCT // Jump on top item zero to start of the block
%token TCC // Jump on nth item zero to start of the block
%token TCG // Jump on bottom item zero to start of the block
%token TGA // Jump unconditionally to end of the block
%token TGT // Jump on top item zero to end of the block
%token TGC // Jump on nth item zero to end of the block
%token TGG // Jump on bottom item zero to end of the block

%token CAA // Print the whole stack
%token CAT // Print top n items of stack
%token CAC // Print top item of the stack
%token CAG // Print bottom n items of stack
%token CTA // Print the whole stack (ASCII)
%token CTT // Print top n items of stack (ASCII)
%token CTC // Print top item of stack (ASCII)
%token CTG // Print bottom n items of stack (ASCII)

%token CCA // Read input to n given by top item of stack
%token CCT // Read input to top nth of the stack
%token CCC // Read input to top of the stack
%token CCG // Read input to bottom nth of the stack

%token CGA // Move to the top of the stack
%token CGT // Move down the stack by n (If nothing, n=1)
%token CGC // Move up the stack by n (If nothing, n=1)
%token CGG // End program

%token GAA // Hex 0
%token GAT // Hex 1
%token GAC // Hex 2
%token GAG // Hex 3
%token GTA // Hex 4
%token GTT // Hex 5
%token GTC // Hex 6
%token GTG // Hex 7
%token GCA // Hex 8
%token GCT // Hex 9
%token GCC // Hex a
%token GCG // Hex b
%token GGA // Hex c
%token GGT // Hex d
%token GGC // Hex e
%token GGG // Hex f

%%

program: block end;

end:	/*empty*/
	| CGG { exit(0); }
	| CGG number { exit($2); }
	;

block: start stmts stop;

stmts: /*empty*/
	| stmts stmt
	;

stmt:	block
	| AAA { insert_instr(1,-1); dup_stack(); }
	| AAT number { insert_instr(2,$2); dup_stack_n(1,$2); }
	| AAC { insert_instr(3,1); copy_nth(1,1); }
	| AAG number { insert_instr(4,$2); dup_stack_n(0,$2); }
	| ATA number { insert_instr(5,$2); push_number_into_stack($2); }
	| ATT number { insert_instr(6,$2); pop_numbers_from_stack(1,$2); }
	| ATC { insert_instr(7,-1); pop_number(); }
	| ATG number { insert_instr(8,$2); pop_numbers_from_stack(0,$2); }
	| ACA { insert_instr(9,-1); clear_stack(); }
	| ACT number { insert_instr(9,$2); copy_nth(1,$2); }
	| ACC number { insert_instr(10,$2); slide($2); }
	| ACG number { insert_instr(11,$2); copy_nth(0,$2); }
	| AGA { insert_instr(12,-1); reverse_whole(); }
	| AGT number { insert_instr(13,$2); reverse_stack_n(1,$2); }
	| AGC { insert_instr(14,2); reverse_stack_n(1,2); }
	| AGG number { insert_instr(15,$2); reverse_stack_n(0,$2); }
	| TAA { insert_instr(16,-1); arith(1); }
	| TAT { insert_instr(17,-1); arith(2); }
	| TAC { insert_instr(18,-1); arith(3); }
	| TAG { insert_instr(19,-1); arith(4); }
	| TTA { insert_instr(20,1); crement(1,1); }
	| TTA number { insert_instr(21,$2); crement(1,$2); }
	| TTT { insert_instr(22,1); crement(0,1); }
	| TTT number { insert_instr(22,$2); crement(0,$2); }
	| CAA { insert_instr(23,-1); print_whole_stack(1); }
	| CAT number { insert_instr(24,$2); print_stack_n(1,1,$2); }
	| CAC { insert_instr(25,1); print_stack_n(1,1,1); }
	| CAG number { insert_instr(26,$2); print_stack_n(0,1,$2); }
	| CTA { insert_instr(27,-1); print_whole_stack(0); }
	| CTT number { insert_instr(28,$2); print_stack_n(1,0,$2); }
	| CTC { insert_instr(29,1); print_stack_n(1,0,1); }
	| CTG number { insert_instr(30,$2); print_stack_n(0,0,$2); }
	| CCA { insert_instr(31,-1); read_top(); }
	| CCT number { insert_instr(32,$2); read_n(1,$2); }
	| CCC { insert_instr(33,1); read_n(1,1); }
	| CCG number { insert_instr(34,$2); read_n(0,$2); }
	| CGA { insert_instr(35,-1); move_top(); }
	| CGT { insert_instr(36,1); move(1,1); }
	| CGT number { insert_instr(37,$2); move(1,$2); }
	| CGC { insert_instr(38,1); move(0,1); }
	| CGC number { insert_instr(38,$2); move(0,$2); }
	;

number: hex { $$=$1; }
	| hex hex { $$=16*($1)+$2; }
	| hex hex hex { $$=16*(16*($1)+$2)+$3; }
	| hex hex hex hex { $$=16*(16*(16*($1)+$2)+$3)+$4; }
	| hex hex hex hex hex { $$=16*(16*(16*(16*($1)+$2)+$3)+$4)+$5; }
	| hex hex hex hex hex hex { $$=16*(16*(16*(16*(16*($1)+$2)+$3)+$4)+$5)+$6; }
	| hex hex hex hex hex hex hex { $$=16*(16*(16*(16*(16*(16*($1)+$2)+$3)+$4)+$5)+$6)+$7; }
	| hex hex hex hex hex hex hex hex { $$=16*(16*(16*(16*(16*(16*(16*($1)+$2)+$3)+$4)+$5)+$6)+$7)+$8; }
	;

hex:	GAA { $$=0; }
	| GAT { $$=1; }
	| GAC { $$=2; }
	| GAG { $$=3; }
	| GTA { $$=4; }
	| GTT { $$=5; }
	| GTC { $$=6; }
	| GTG { $$=7; }
	| GCA { $$=8; }
	| GCT { $$=9; }
	| GCC { $$=10; }
	| GCG { $$=11; }
	| GGA { $$=12; }
	| GGT { $$=13; }
	| GGC { $$=14; }
	| GGG { $$=15; }
	;

start: TTC {
		/* Start block */
	};

stop: TTG {
		/* End block */
	};

%%

int yywrap(void) {
	return 1;
}

void count(void) {
	int i;
	for(i=0;i<strlen(yytext);i++) {
		if(yytext[i]=='\n') {
			colNum=1;
			lineNum++;
		} else
			colNum++;
	}
}

void yyerror(char* msg) {
	if(strlen(yytext)) {
		printf("Error: Line %4.4d, Column %4.4d-%4.4d: %s\n",lineNum,(colNum-yyleng>0?colNum-yyleng:0),colNum-1,msg);
	} else {
		printf("%*s\n",0,"^");
		printf("Error: Line %d:Unexpected EOF found\n",lineNum);
	}
}

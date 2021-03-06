#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

extern FILE* yyin;

extern int yyparse(void);
extern void yyerror(char *);

typedef struct mem {
	int v;
	struct mem *down;
	struct mem *up;
} *gmem;

typedef struct ins {
	int t;
	int n;
	struct ins *after;
} *gins;

typedef struct blk {
	struct ins *link;
	struct blk *next;
} *gblk;

gmem top = NULL;
gmem bottom = NULL;

gins first = NULL;
gins last = NULL;

gblk blks = NULL;

/*
 * Insert an instruction
 */
void insert_instr(int type, int num) {
	gins tmp = malloc(sizeof(gins));

	tmp->t = type;
	tmp->n = num;
	tmp->after = NULL;

	if (last == NULL) {
		first = tmp;
	} else {
		last->after = tmp;
	}

	last = tmp;
	return;
}

/*
 * Push a block on the stack
 */
void push_blk(gins instr) {
	gblk tmp = malloc(sizeof(gblk));

	tmp->link = instr;
	tmp->next = blks;

	blks = tmp;
	return;
}

/*
 * Pop a block from the stack
 */
void pop_blk(void) {
	gblk tmp;
	tmp = blks;

	if (tmp != NULL) {
		blks = blks->next;
		free(tmp);
	} else {
		yyerror("Stack underflow for blocks");
	}

	return;
}

/*
 * Clear the instruction stack
 */
void clear_instr(void) {
	gins tmp, buf;
	tmp = first;
	first = NULL;
	last = NULL;

	while (tmp != NULL) {
		buf = tmp;
		tmp = tmp->after;
		free(buf);
	}

	return;
}

/*
 * ACA -- Clear the whole stack
 */
void clear_stack(void) {
	gmem tmp, buf;
	tmp = top;
	top = NULL;
	bottom = NULL;

	while (tmp != NULL) {
		buf = tmp;
		tmp = tmp->down;
		free(buf);
	}

	return;
}

/*
 * ACC -- Slide n items keeping top item
 */
void slide(int n) {
	int i;
	gmem tmp, buf;
	tmp = top->down;

	for (i = 0; i < n; i++) {
		if (tmp != NULL) {
			buf = tmp;
			tmp = tmp->down;
			free(buf);
		} else {
			break;
		}
	}

	top->down = tmp;

	if(tmp != NULL) {
		tmp->up = top;
	} else {
		bottom = top;
	}

	return;
}

/*
 * ATA -- Push number onto stack
 */
void push_number_into_stack(int n) {
	gmem tmp = malloc(sizeof(gmem));
	tmp->v = n;
	tmp->up = NULL;

	if (top == NULL) {
		top = tmp;
		bottom = tmp;
		tmp->down = NULL;
	} else {
		tmp->down = top;
		top->up = tmp;
		top = tmp;
	}

	return;
}

/*
 * ATC -- Pop number from stack (Discarding)
 */
void pop_number(void) {
	gmem tmp;
	tmp = top;
	top = tmp->down;

	if (top != NULL) {
		top->up = NULL;
	}

	free(tmp);
	return;
}

/*
 * ATT -- Pop top n items from stack
 * ATG -- Pop bottom n items from stack
 */
void pop_numbers_from_stack(int f, int n) {
	int i;
	gmem tmp;

	for (i = 0; i < n; i++) {
		if (f == 1) {
			tmp = top;
			top = tmp->down;
			free(tmp);

			if (top != NULL) {
				top->up = NULL;
			} else {
				yyerror("Number of elements in the stack is less than required number");
			}
		} else {
			tmp = bottom;
			bottom = tmp->up;
			free(tmp);

			if (bottom != NULL) {
				bottom->down = NULL;
			} else {
				yyerror("Number of elements in the stack is less than required number");
			}
		}
	}

	return;
}

/*
 * AAA -- Duplicate the whole stack
 */
void dup_stack(void) {
	gmem tmp, buf;
	tmp = bottom;
	buf = top;

	while (tmp != buf) {
		push_number_into_stack(tmp->v);
		tmp = tmp->up;
	}

	push_number_into_stack(buf->v);
	return;
}

/*
 * AAT -- Duplicate top n items on the stack
 * AAG -- Duplicate bottom n items on the stack
 */
void dup_stack_n(int f, int n) {
	int i;
	gmem tmp, buf, top2, bottom2;
	tmp = (f == 1 ? top : bottom);
	top2 = top;
	bottom2 = bottom;

	for (i = 0; i < n; i++) {
		if (tmp == NULL) {
			break;
		}

		if (f == 1) {
			buf = malloc(sizeof(gmem));
			buf->v = tmp->v;
			buf->down = top2;
			buf->up = top2->up;

			if (top2->up != NULL) {
				top2->up->down = buf;
			} else {
				top = buf;
			}

			top2->up = buf;
			tmp = tmp->down;
		} else {
			buf = malloc(sizeof(gmem));
			buf->v = tmp->v;
			buf->up = bottom2;
			buf->down = bottom2->down;

			if (bottom2->down != NULL) {
				bottom2->down->up = buf;
			} else {
				bottom = buf;
			}

			bottom2->down = buf;
			tmp = tmp->up;
		}
	}

	if (tmp == NULL) {
		yyerror("Number of elements in the stack is less than the required number");
	}

	return;
}

/*
 * AAC -- Duplicate top item on the stack
 * ACT -- Copy top nth item on the stack
 * ACG -- Copy bottom nth item on the stack
 */
void copy_nth(int f, int n) {
	int i;
	gmem tmp;
	tmp = (f == 1 ? top : bottom);

	for (i = 1; i < n; i++) {
		if (tmp == NULL) {
			break;
		}

		tmp = (f == 1 ? tmp->down : tmp->up);
	}

	if (tmp == NULL) {
		yyerror("Number of elements in the stack is less than the required number");
	} else {
		push_number_into_stack(tmp->v);
	}

	return;
}

/*
 * TAA -- Addition 1+2 items and push the result
 * TAT -- Subtraction 2-1 items and push the result
 * TAC -- Multiplication 2*1 items and push the result
 * TAG -- Division 2/1 items and push the result
 */
void arith(int m) {
	int r;

	if(top == NULL || top->down == NULL) {
		yyerror("Number of elements in the stack is less than the required elements");
	}

	switch(m) {
		case 1:
			r = top->v + top->down->v;
			break;
		case 2:
			r = top->down->v - top->v;
			break;
		case 3:
			r = top->v * top->down->v;
			break;
		case 4:
		{
			if(top->v == 0) {
				yyerror("Division by zero not possible");
				r = top->down->v;
			} else {
				r = top->down->v / top->v;
			}

			break;
		}
	}

	push_number_into_stack(r);
	return;
}

/*
 * TCA -- Jump uncoditionally to start of the block
 */
gins jmp_start(void) {
	if (blks == NULL || blks->link == NULL) {
		yyerror("Block arrangments fault");
	}

	return blks->link;
}

/*
 * TCT -- Jump on top item zero to start of the block
 * TCC -- Jump on nth item zero to start of the block
 * TCG -- Jump on bottom item zero to start of the block
 */
gins jmp_start_nth(int f, int n, gins buf) {
	int i;
	gmem tmp;
	tmp = (f == 1 ? top : bottom);

	for(i = 1; i < n; i++) {
		if(tmp == NULL) {
			break;
		}

		tmp = (f == 1 ? tmp->down : tmp->up);
	}

	if(tmp == NULL) {
		yyerror("Number of elements in the stack is less than the required number");
	} else if (tmp->v == 0) {
		return jmp_start();
	}

	return buf;
}

/*
 * TGA -- Jump uncoditionally to end of the block
 */
gins jmp_end(void) {
	int i = 1;
	gins tmp;

	if (blks == NULL || blks->link == NULL) {
		yyerror("Block arrangement fault");
	} else {
		tmp = blks->link;
	}

	while (1) {
		tmp = tmp->after;

		if (tmp == NULL) {
			yyerror("No instruction after the end of block");
		} else if (tmp->t == 30) {
			i = i + 1;
		} else if (tmp->t == 31) {
			i = i - 1;

			if (i == 0) {
				break;
			}
		}
	}

	return tmp;
}

/*
 * TGT -- Jump on top item zero to end of the block
 * TGC -- Jump on nth item zero to end of the block
 * TGG -- Jump on bottom item zero to end of the block
 */
gins jmp_end_nth(int f, int n, gins buf) {
	int i;
	gmem tmp;
	tmp = (f == 1 ? top : bottom);

	for (i = 1; i < n; i++) {
		if (tmp == NULL) {
			break;
		}

		tmp = (f == 1 ? tmp->down : tmp->up);
	}

	if (tmp == NULL) {
		yyerror("Number of elements in the stack is less than the required number");
	} else if (tmp->v == 0) {
		return jmp_end();
	}

	return buf;
}

/*
 * TTA -- Increment the top item by n (if nothing n=1)
 * TTT -- Decrement the top item by n (if nothing n=1)
 */
void crement(int f, int n) {
	top->v = (f == 1 ? (top->v + n) : (top->v - n));
	return;
}

/*
 * CAA -- Print the whole stack
 * CTA -- Print the whole stack (ASCII)
 */
void print_whole_stack(int a) {
	gmem tmp = bottom;

	while (tmp != NULL) {
		if (a == 1) {
			printf("%d", tmp->v);
		} else {
			printf("%c", tmp->v);
		}

		tmp = tmp->up;
	}

	return;
}

/*
 * CAT -- Print top n items of stack
 * CAC -- Print top item of the stack
 * CAG -- Print bottom n items of stack
 * CTT -- Print top n items of stack (ASCII)
 * CTC -- Print top item of stack (ASCII)
 * CTG -- Print bottom n items of stack (ASCII)
 */
void print_stack_n(int f, int a, int n) {
	int i, flag = 0;
	gmem tmp;
	tmp = (f == 1 ? top : bottom);

	if (f == 1) {
		for (i = 1; i < n; i++) {
			if (tmp == NULL) {
				yyerror("Number of elements in the stack is less than the required number");
				break;
			}

			tmp = tmp->down;
		}
	}

	for (i = 0; i < n; i++) {
		if (tmp == NULL) {
			flag = 1;
			break;
		}

		if(a == 1) {
			printf("%d", tmp->v);
		} else {
			printf("%c", tmp->v);
		}

		tmp = tmp->up;
	}

	if (flag == 1) {
		yyerror("Number of elements in the stack is less than the required number");
	}

	return;
}

/*
 * AGA -- Reverse the whole stack
 */
void reverse_whole(void) {
	gmem top2, bottom2, tmp, buf;
	top2 = bottom;
	bottom2 = top;

	bottom2->up = top->down;
	bottom2->down = NULL;

	tmp = bottom2->up;

	while (tmp != NULL) {
		buf = tmp->down;
		tmp->down = tmp->up;
		tmp->up = buf;
		tmp = buf;
	}

	top = top2;
	bottom = bottom2;
	return;
}

/*
 * AGT -- Reverse the top n items
 * AGC -- Reverse the top 2 items
 * AGG -- Reverse the bottom n items
 */
void reverse_stack_n(int f, int n) {
	int i;
	gmem tmp, buf, tmp2;
	tmp = (f == 1 ? top : bottom);
	tmp2 = tmp;

	for (i = 0; i < n-1; i++) {
		if (tmp == NULL) {
			break;
		}

		if (f == 1) {
			buf = tmp->down;
			tmp->down = tmp->up;
			tmp->up = buf;
		} else {
			buf = tmp->up;
			tmp->up = tmp->down;
			tmp->down = buf;
		}

		tmp = buf;
	}

	if (tmp != NULL) {
		if (f == 1) {
			tmp->down->up = tmp2;
			tmp2->down = tmp->down;
			tmp->down = tmp->up;
			tmp->up = NULL;
			top = tmp;
		} else {
			tmp->up->down = tmp2;
			tmp2->up = tmp->up;
			tmp->up = tmp->down;
			tmp->down = NULL;
			bottom = tmp;
		}
	}

	return;
}

/*
 * CGA -- Move to the top of the stack
 */
void move_top(void) {
	gmem tmp, buf;
	tmp = top;
	buf = top;

	while (tmp != NULL) {
		buf = tmp;
		tmp = tmp->up;
	}

	top = buf;
	return;
}

/*
 * CGT -- Move down the stack by n (If nothing, n=1)
 * CGC -- Move up the stack by n (If nothing, n=1)
 */
void move(int f, int n) {
	int i;

	for (i = 0; i < n; i++) {
		if (top != NULL) {
			top = (f == 1 ? top->down : top->up);
		} else {
			yyerror("Number of elements in the stack is less than the required number");
		}
	}

	return;
}

/*
 * CCT -- Read input to top nth of the stack
 * CCC -- Read input to top of the stack
 * CCG -- Read input to bottom nth of the stack
 */
void read_n(int f, int n) {
	int i;
	char in;
	gmem tmp;

	if(top == NULL) {
		push_number_into_stack(32);
	}

	tmp = (f == 1 ? top : bottom);

	for(i = 1; i < n; i++) {
		if(tmp == NULL) {
			break;
		}

		tmp = (f == 1 ? tmp->down : tmp->up);
	}

	if (tmp != NULL) {
		scanf("%c", &in);
		tmp->v = (int)in;
	} else {
		yyerror("Number of elements in the stack is less than the required number");
	}

	return;
}

/*
 * CCA -- Read input to n given by top item of stack
 */
void read_top(void) {
	read_n(1, top->v + 1);
	return;
}

/*
 * Execute the instructions
 */
void execute_instr(void) {
	gins buf;

	if (first != NULL) {
		buf = first;
	} else {
		return;
	}

	while (1) {
		switch (buf->t) {
			case 0:
				exit(buf->n);
				break;
			case 1:
				dup_stack();
				break;
			case 2:
				dup_stack_n(1, buf->n);
				break;
			case 3:
				copy_nth(1, 1);
				break;
			case 4:
				dup_stack_n(0, buf->n);
				break;
			case 5:
				push_number_into_stack(buf->n);
				break;
			case 6:
				pop_numbers_from_stack(1, buf->n);
				break;
			case 7:
				pop_number();
				break;
			case 8:
				pop_numbers_from_stack(0, buf->n);
				break;
			case 9:
				clear_stack();
				break;
			case 10:
				copy_nth(1, buf->n);
				break;
			case 11:
				slide(buf->n);
				break;
			case 12:
				copy_nth(0, buf->n);
				break;
			case 13:
				reverse_whole();
				break;
			case 14:
				reverse_stack_n(1, buf->n);
				break;
			case 15:
				reverse_stack_n(0, buf->n);
				break;
			case 16:
				arith(buf->n);
				break;
			case 17:
				crement(1, buf->n);
				break;
			case 18:
				crement(0, buf->n);
				break;
			case 19:
				print_whole_stack(buf->n);
				break;
			case 20:
				print_stack_n(1, 1, buf->n);
				break;
			case 21:
				print_stack_n(0, 1, buf->n);
				break;
			case 22:
				print_stack_n(1, 0, buf->n);
				break;
			case 23:
				print_stack_n(0, 0, buf->n);
				break;
			case 24:
				read_top();
				break;
			case 25:
				read_n(1, buf->n);
				break;
			case 26:
				read_n(0, buf->n);
				break;
			case 27:
				move_top();
				break;
			case 28:
				move(1, buf->n);
				break;
			case 29:
				move(0, buf->n);
				break;
			case 30:
				push_blk(buf);
				break;
			case 31:
				pop_blk();
				break;
			case 32:
				buf = jmp_start();
				break;
			case 33:
				buf = jmp_start_nth(1, buf->n, buf);
				break;
			case 34:
				buf = jmp_start_nth(0, 1, buf);
				break;
			case 35:
				buf = jmp_end();
				break;
			case 36:
				buf = jmp_end_nth(1, buf->n, buf);
				break;
			case 37:
				buf = jmp_end_nth(0, 1, buf);
				break;
			default:
				break;
		}

		if (buf->after == NULL) {
			break;
		} else {
			buf = buf->after;
		}
	}
}

/*
 * Execute the program
 */
void execute(void) {
	execute_instr();
	clear_instr();
	clear_stack();

	return;
}

/*
 * Main top level function for genome
 */
int main(int argc, char**argv) {
	if(argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	yyin = NULL;
	yyin = fopen(argv[1], "r");

	if (yyin == NULL) {
		printf("%s: %s: No such file or directory\n", argv[0], argv[1]);
		return 1;
	} else {
		yyparse();
		execute();
	}

	fclose(yyin);
	printf("\n");

	return 0;
}

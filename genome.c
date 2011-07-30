#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

extern int yyleng;
extern FILE* yyin;
extern char* yytext;

extern int yyparse(void);
extern int yywrap(void);
extern void yyerror(char *);

typedef struct node {
	int v;
	struct node  *down;
	struct node *up;
} *gnode;

gnode top=NULL;
gnode bottom=NULL;

void clear_stack(void) {
	gnode tmp, buf;
	tmp=top;
	top=NULL;
	bottom=NULL;
	while(tmp!=NULL) {
		buf=tmp;
		tmp=tmp->down;
		free(buf);
	}
	return;
}

void slide(int n) {
	int i;
	gnode tmp, buf;
	tmp=top->down;
	for(i=0;i<n;i++) {
		if(tmp!=NULL) {
			buf=tmp;
			tmp=tmp->down;
			free(buf);
		} else
			break;
	}
	top->down=tmp;
	if(tmp!=NULL)
		tmp->up=top;
	else
		bottom=top;
	return;
}

void push_number_into_stack(int n) {
	gnode tmp=malloc(sizeof(gnode));
	tmp->v=n;
	tmp->up=NULL;
	if(top==NULL) {
		top=tmp;
		bottom=tmp;
		tmp->down=NULL;
	} else {
		tmp->down=top;
		top->up=tmp;
		top=tmp;
	}
	return;
}

void pop_number(void) {
	gnode tmp;
	tmp=top;
	top=tmp->down;
	if(top!=NULL)
		top->up=NULL;
	free(tmp);
	return;
}

void pop_numbers_from_stack(int f, int n) {
	int i;
	gnode tmp;
	for(i=0;i<n;i++) {
		if(tmp==NULL)
			break;
		if(f==1) {
			tmp=top;
			top=tmp->down;
			if(top!=NULL)
				top->up=NULL;
			free(tmp);
		} else {
			tmp=bottom;
			bottom=tmp->up;
			if(bottom!=NULL)
				bottom->down=NULL;
			free(tmp);
		}
	}
	if(tmp==NULL)
		yyerror("Number of elements in the stack is less than required number");
	return;
}

void dup_stack(void) {
	gnode tmp, buf;
	tmp=bottom;
	buf=top;
	while(tmp!=buf) {
		push_number_into_stack(tmp->v);
		tmp=tmp->up;
	}
	push_number_into_stack(buf->v);
	return;
}

void dup_stack_n(int f, int n) {
	int i;
	gnode tmp, buf, top2, bottom2;
	tmp=(f==1)?top:bottom;
	top2=top;
	bottom2=bottom;
	for(i=0;i<n;i++) {
		if(tmp==NULL)
			break;
		if(f==1) {
			buf=malloc(sizeof(gnode));
			buf->v=tmp->v;
			buf->down=top2;
			buf->up=top2->up;
			if(top2->up!=NULL)
				top2->up->down=buf;
			else
				top=buf;
			top2->up=buf;
			tmp=tmp->down;
		} else {
			buf=malloc(sizeof(gnode));
			buf->v=tmp->v;
			buf->up=bottom2;
			buf->down=bottom2->down;
			if(bottom2->down!=NULL)
				bottom2->down->up=buf;
			else
				bottom=buf;
			bottom2->down=buf;
			tmp=tmp->up;
		}
	}
	if(tmp==NULL)
		yyerror("Number of elements in the stack is less than the required number");
	return;
}

void copy_nth(int f, int n) {
	int i;
	gnode tmp;
	tmp=(f==1)?top:bottom;
	for(i=1;i<n;i++) {
		if(tmp==NULL)
			break;
		tmp=(f==1)?tmp->down:tmp->up;
	}
	if(tmp==NULL)
		yyerror("Number of elements in the stack is less than the required number");
	else
		push_number_into_stack(tmp->v);
	return;
}

void arith(int m) {
	int r;
	if(top==NULL || top->down==NULL)
		yyerror("Number of elements in the stack is less than the required elements");
	switch(m) {
		case 1:
			r=top->v+top->down->v;
			break;
		case 2:
			r=top->down->v-top->v;
			break;
		case 3:
			r=top->v*top->down->v;
			break;
		case 4:
			if(top->v==0) {
				yyerror("Division by zero not possible");
				r=top->down->v;
			} else
				r=top->down->v/top->v;
			break;
	}
	push_number_into_stack(r);
	return;
}

void crement(int f, int n) {
	top->v=(f==1)?(top->v+n):(top->v-n);
	return;
}

void print_whole_stack(int a) {
	gnode tmp=bottom;
	while(tmp!=NULL) {
		if(a==1)
			printf("%d",tmp->v);
		else
			printf("%c",tmp->v);
		tmp=tmp->up;
	}
	return;
}

void print_stack_n(int f, int a, int n) {
	int i, flag=0;
	gnode tmp;
	tmp=(f==1)?top:bottom;
	if(f==1) {
		for(i=1;i<n;i++) {
			if(tmp==NULL) {
				yyerror("Number of elements in the stack is less than the required number");
				break;
			}
			tmp=tmp->down;
		}
	}
	for(i=0;i<n;i++) {
		if(tmp==NULL) {
			flag=1;
			break;
		}
		if(a==1)
			printf("%d",tmp->v);
		else
			printf("%c",tmp->v);
		tmp=tmp->up;
	}
	if(flag==1)
		yyerror("Number of elements in the stack is less than the required number");
	return;
}

void reverse_whole(void) {
	gnode top2, bottom2, tmp, buf;
	top2 = bottom;
	bottom2 = top;
	bottom2->up = top->down;
	bottom2->down = NULL;
	tmp = bottom2->up;
	while(tmp!=NULL) {
		buf = tmp->down;
		tmp->down = tmp->up;
		tmp->up = buf;
		tmp = buf;
	}
	top = top2;
	bottom = bottom2;
	return;
}

void reverse_stack_n(int f, int n) {
	int i;
	gnode tmp, buf, tmp2;
	tmp=(f==1)?top:bottom;
	tmp2 = tmp;
	for(i=0;i<n-1;i++) {
		if(tmp==NULL)
			break;
		if(f==1) {
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
	if(tmp!=NULL) {
		if(f==1) {
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

void move_top(void) {
	gnode tmp, buf;
	tmp=top;
	buf=top;
	while(tmp!=NULL) {
		buf=tmp;
		tmp=tmp->up;
	}
	top=buf;
	return;
}

void move(int f, int n) {
	int i;
	for(i=0;i<n;i++) {
		if(top!=NULL)
			top=(f==1)?top->down:top->up;
		else
			yyerror("Number of elements in the stack is less than the required number");
	}
	return;
}

void read_n(int f, int n) {
	int i;
	char in;
	gnode tmp;
	if(top==NULL)
		push_number_into_stack(32);
	tmp=(f==1)?top:bottom;
	for(i=1;i<n;i++) {
		if(tmp==NULL)
			break;
		tmp=(f==1)?tmp->down:tmp->up;
	}
	if(tmp!=NULL) {
		scanf("%c",&in);
		tmp->v = (int)in;
	} else
		yyerror("Number of elements in the stack is less than the required number");
	return;
}

void read_top(void) {
	read_n(1,top->v+1);
	return;
}

int main(int argc, char**argv) {
	if(argc!=2) {
		printf("Usage: %s <filename>\n",argv[0]);
		return 1;
	}
	yyin=NULL;
	yyin=fopen(argv[1],"r");
	if(yyin==NULL) {
		printf("%s: %s: No such file or directory\n",argv[0],argv[1]);
		return 1;
	} else
		yyparse();
	fclose(yyin);
	printf("\n");
	return 0;
}

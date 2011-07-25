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
	struct node  *next;
	struct node *prev;
} *gnode;

gnode head=NULL;
gnode tail=NULL;

void clear_stack(void) {
	gnode tmp, buf;
	tmp=head;
	head=NULL;
	tail=NULL;
	while(tmp!=NULL) {
		buf=tmp;
		tmp=tmp->next;
		free(buf);
	}
	return;
}

void slide(int n) {
	int i;
	gnode tmp, buf;
	tmp=head->next;
	for(i=0;i<n;i++) {
		if(tmp!=NULL) {
			buf=tmp;
			tmp=tmp->next;
			free(buf);
		} else
			break;
	}
	head->next=tmp;
	if(tmp!=NULL)
		tmp->prev=head;
	else
		tail=head;
	return;
}

void push_number_into_stack(int n) {
	gnode tmp=malloc(sizeof(gnode));
	tmp->v=n;
	tmp->prev=NULL;
	if(head==NULL) {
		head=tmp;
		tail=tmp;
		tmp->next=NULL;
	} else {
		tmp->next=head;
		head->prev=tmp;
		head=tmp;
	}
	return;
}

void pop_number(void) {
	gnode tmp;
	tmp=head;
	head=tmp->next;
	if(head!=NULL)
		head->prev=NULL;
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
			tmp=head;
			head=tmp->next;
			if(head!=NULL)
				head->prev=NULL;
			free(tmp);
		} else {
			tmp=tail;
			tail=tmp->prev;
			if(tail!=NULL)
				tail->next=NULL;
			free(tmp);
		}
	}
	if(tmp==NULL)
		yyerror("Number of elements in the stack is less than required number");
	return;
}

void dup_stack(void) {
	gnode tmp, buf;
	tmp=tail;
	buf=head;
	while(tmp!=buf) {
		push_number_into_stack(tmp->v);
		tmp=tmp->prev;
	}
	push_number_into_stack(buf->v);
	return;
}

void dup_stack_n(int f, int n) {
	int i;
	gnode tmp, buf, head2, tail2;
	tmp=(f==1)?head:tail;
	head2=head;
	tail2=tail;
	for(i=0;i<n;i++) {
		if(tmp==NULL)
			break;
		if(f==1) {
			buf=malloc(sizeof(gnode));
			buf->v=tmp->v;
			buf->next=head2;
			buf->prev=head2->prev;
			if(head2->prev!=NULL)
				head2->prev->next=buf;
			else
				head=buf;
			head2->prev=buf;
			tmp=tmp->next;
		} else {
			buf=malloc(sizeof(gnode));
			buf->v=tmp->v;
			buf->prev=tail2;
			buf->next=tail2->next;
			if(tail2->next!=NULL)
				tail2->next->prev=buf;
			else
				tail=buf;
			tail2->next=buf;
			tmp=tmp->prev;
		}
	}
	if(tmp==NULL)
		yyerror("Number of elements in the stack is less than the required number");
	return;
}

void copy_nth(int f, int n) {
	int i;
	gnode tmp;
	tmp=(f==1)?head:tail;
	for(i=1;i<n;i++) {
		if(tmp==NULL)
			break;
		tmp=(f==1)?tmp->next:tmp->prev;
	}
	if(tmp==NULL)
		yyerror("Number of elements in the stack is less than the required number");
	else
		push_number_into_stack(tmp->v);
	return;
}

void arith(int m) {
	int r;
	if(head==NULL || head->next==NULL)
		yyerror("Number of elements in the stack is less than the required elements");
	switch(m) {
		case 1:
			r=head->v+head->next->v;
			break;
		case 2:
			r=head->next->v-head->v;
			break;
		case 3:
			r=head->v*head->next->v;
			break;
		case 4:
			if(head->v==0) {
				yyerror("Division by zero not possible");
				r=head->next->v;
			} else
				r=head->next->v/head->v;
			break;
	}
	push_number_into_stack(r);
	return;
}

void crement(int f, int n) {
	head->v=(f==1)?(head->v+n):(head->v-n);
	return;
}

void print_whole_stack(int a) {
	gnode tmp=tail;
	while(tmp!=NULL) {
		if(a==1)
			printf("%d",tmp->v);
		else
			printf("%c",tmp->v);
		tmp=tmp->prev;
	}
	return;
}

void print_stack_n(int f, int a, int n) {
	int i, flag=0;
	gnode tmp;
	tmp=(f==1)?head:tail;
	if(f==1) {
		for(i=1;i<n;i++) {
			if(tmp==NULL) {
				yyerror("Number of elements in the stack is less than the required number");
				break;
			}
			tmp=tmp->next;
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
		tmp=tmp->prev;
	}
	if(flag==1)
		yyerror("Number of elements in the stack is less than the required number");
	return;
}

void reverse_whole(void) {
	
	return;
}

void reverse_stack_n(int f, int n) {
	
	return;
}

void move_top(void) {
	gnode tmp, buf;
	tmp=head;
	buf=head;
	while(tmp!=NULL) {
		buf=tmp;
		tmp=tmp->prev;
	}
	head=buf;
	return;
}

void move(int f, int n) {
	int i;
	for(i=0;i<n;i++) {
		if(head!=NULL)
			head=(f==1)?head->next:head->prev;
		else
			yyerror("Number of elements in the stack is less than the required number");
	}
	return;
}

void read_n(int f, int n) {
	int i;
	gnode tmp;
	tmp=(f==1)?head:tail;
	for(i=1;i<n;i++) {
		if(tmp==NULL)
			break;
		tmp=(f==1)?tmp->next:tmp->prev;
	}
	if(tmp!=NULL) {
		scanf("%d",&(tmp->v));
	} else
		yyerror("Number of elements in the stack is less than the required number");
	return;
}

void read_top(void) {
	read_n(1,head->v);
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

/*
+ JMJ +

Emin's Lisp
A little implementation of Lisp, made by me with the help of buildyourownlisp.com

Emin Aksoy
2025 A.D.

*/

#define VERSION "0.0.4"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpc.h"

#ifdef _WIN32
#include <string.h>

static char buffer[2048]; 

/* Placeholder readline function for Windows */
char* readline(char* prompt){
	fputs(prompt, stdout);
	fgets(buffer, 2048, stdin);
	char* cpy = malloc(strlen(buffer) + 1);
	strcpy(cpy, buffer);
	cpy[strlen(cpy) - 1] = '\0';
	return cpy; 
}

/* Placeholder add_history function, make this do something */
void add_history(char* unused){}

#else
#include <editline/readline.h>
#endif

long eval_op(long x, char* op, long y){
	if (strcmp(op, "^") == 0) {return (long) pow((double) x, (double) y);}
	if (strcmp(op, "+") == 0) {return x + y;}
	if (strcmp(op, "-") == 0) {return x - y;}			
	if (strcmp(op, "*") == 0) {return x * y;}
	if (strcmp(op, "/") == 0) {return x / y;}	
	if (strcmp(op, "%") == 0) {return x % y;}
	return 0;
}

long eval(mpc_ast_t* t){

	/* If the tag is a number, it has no children so immediately return the number	*/
	if(strstr(t->tag, "number")){
		return atoi(t->contents);
	}

	/* The second child is always the operator */
	char* op = t->children[1]->contents;

	/* Evaluate the third child and store in x */
	long x = eval(t->children[2]);

	/* Special case for - operator and one argument; Very unelegant, fix this */
	if(strstr(op, "-") && !strstr(t->children[3]->tag, "expr")){
		return x * -1;
	}

	/* Iterate the remaining children and combine them with x */
	int i = 3;
	while(strstr(t->children[i]->tag, "expr")){
		x = eval_op(x, op, eval(t->children[i]));
		i++;
	}
	
	return x;
}

/* Calculates the number of leaves in the given tree */
int num_l(mpc_ast_t* t){
	if(t->children_num == 0){
		return 0;
	} 
	else {
		int children_total = 0;	
		
		for(int i = 0; i < t->children_num; i++){
			children_total += num_l(t->children[i]);
		}	
		return children_total + t->children_num;
	}
}

/* Calculates the number of branches in the given tree */
int num_b(mpc_ast_t* t){
	/* Do stuff */
}

int main(int argc, char* argv[]){

	/* Parsers Declared */
	mpc_parser_t* Number = mpc_new("number");
	mpc_parser_t* Operator = mpc_new("operator");
	mpc_parser_t* Expression = mpc_new("expression");
	mpc_parser_t* Elisp = mpc_new("elisp");

	/* Parsers Defined */
	mpca_lang(MPCA_LANG_DEFAULT, 
	"																		\
		number		:	/-?\\d+(\\.\\d+)?/ 				;					\
		operator	:	'+' | '-' | '*' | '/' | '%'	| '^';						\
		expression	:	<number> | '(' <operator> <expression>+ ')' ;		\
		elisp		:	/^/ <operator> <expression>+ /$/ ;					\
	",
	Number, Operator, Expression, Elisp);

	/* Version and exit info */
	printf("Welcome to Elisp, version %s\n", VERSION);
	printf("Press Ctrl+c to exit the interpreter\n");

	while(1){
		char* input = readline("Elisp> ");
		add_history(input);

		/* Begin Parsing */
		mpc_result_t r;
		if(mpc_parse("stdin", input, Elisp, &r)){
			/* Success */
			mpc_ast_print(r.output);
			long result = eval(r.output);
			printf("%li\n", result);
			mpc_ast_delete(r.output);
		} else {
			/* Error */
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}

		free(input);
	}

	mpc_cleanup(4, Number, Operator, Expression, Elisp);

	return 0;
}

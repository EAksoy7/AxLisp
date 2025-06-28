#define VERSION "0.0.3"
#include <stdio.h>
#include <stdlib.h>
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
		operator	:	'+' | '-' | '*' | '/' | '%'	;						\
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

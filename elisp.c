#define VERSION "0.0.2"
#include <stdio.h>
#include <stdlib.h>

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

	/* Version and exit info */
	printf("Welcome to Elisp, version %s\n", VERSION);
	puts("Press Ctrl+c to exit the interpreter");

	while(1){
		char* input = readline("Elisp> ");
		add_history(input);
		printf("You entered: %s\n", input);
		free(input);
	}

	return 0;
}

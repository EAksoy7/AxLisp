#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>
#define VERSION "0.0.2"

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

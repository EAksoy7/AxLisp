#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>

/* Input buffer */
int main(int argc, char* argv[]){

	/* Version and exit info */
	puts("Welcome to Elisp.");
	puts("Press Ctrl+c to exit the interpreter");

	while(1){
		char* input = readline("Elisp> ");
		add_history(input);
		printf("You entered: %s\n", input);
		free(input);
	}

	return 0;
}

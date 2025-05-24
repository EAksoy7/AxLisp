#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>
#include <editline/history.h>

/* Input buffer */
int main(int argc, char* argv[]){

	/* Version and exit info */
	puts("Elisp version 0.0.0.0.1");
	puts("Press Ctrl+c to exit the interpreter");

	while(1){

		char* input = readline("Elisp> ");
		add_history(input);
		printf("You entered: %s", input);
		free(input);
	}

	return 0;
}

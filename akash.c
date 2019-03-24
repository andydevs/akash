/**
 * Andy Kharbandy's Average Shell (Akash)
 *
 * A totally average shell program that
 * is an acronym and only coincidentally
 * also the name of a man
 *
 * Author:  Anshul Kharbanda
 * Created: 3 - 24 - 2019
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>

void print_head(void) {
	printf("\n");
	printf("||                                             ||\n");
	printf("||           ^    |  /    ^    /-- |           ||\n");
	printf("||          / \\   | /    / \\   |_  |--\\        ||\n");
	printf("||         /---\\  | \\   /---\\    \\ |  |        ||\n");
	printf("||        /     \\ |  \\ /     \\ __/ |  |        ||\n");
	printf("||        Andy Kharbandy's Average Shell       ||\n");
	printf("||                                             ||\n");
	printf("||        Type ctrl+D or 'exit' to exit        ||\n");
	printf("||                                             ||\n");
	printf("\n");
}

void handle_exit(char* cmdline) {
	if (!cmdline) {
		printf("Exiting...\n");
		exit(EXIT_SUCCESS);
	}
	if (!strcmp(cmdline, "exit")) {
		printf("Exiting...\n");
		exit(EXIT_SUCCESS);
	}
}

void handle_command(char* cmdline) {
	printf("You typed in command: %s\n", cmdline);
}


/**
 * Program entry point
 *
 * @param argc number of command arguments
 * @param argv command arguments
 * 
 * @return exit status
 */
int main(int argc, const char** argv) {
	print_head();
	char *cmdline;
	while(1) {
		cmdline = readline("$ ");
		handle_exit(cmdline);	
		handle_command(cmdline);
		free(cmdline);
	}
}

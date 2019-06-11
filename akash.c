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
#include "parser.h"
#include "execute.h"

/**
 * Print banner of shell
 */
void print_banner(void) {
	printf("\n");
	printf("||                                             ||\n");
	printf("||          \e[34m ^    |  /    ^    /-- |\e[0m           ||\n");
	printf("||          \e[34m/ \\   | /    / \\   |_  |--\\\e[0m        ||\n");
	printf("||         \e[34m/---\\  | \\   /---\\    \\ |  |\e[0m        ||\n");
	printf("||        \e[34m/     \\ |  \\ /     \\ __/ |  |\e[0m        ||\n");
	printf("||        \e[36mAndy Kharbandy's Average Shell\e[0m       ||\n");
	printf("||                                             ||\n");
	printf("||        \e[95mType ctrl+D or 'exit' to exit\e[0m        ||\n");
	printf("||                                             ||\n");
	printf("\n");
}

/**
 * Handle exit status of command line.
 * Exits if command line is exit.
 *
 * @param cmdline command line to check for exit status
 */
void handle_exit(char* cmdline) {
	if (!cmdline || !strcmp(cmdline, "exit")) {
		printf("Exiting...\n");
		parser_deinit();
		exit(EXIT_SUCCESS);
	}	
}

/**
 * Parse and execute command line
 *
 * @param cmdline command line string to execute 
 */
void handle_command(char* cmdline) {
	struct parse* parse = parse_command_input(cmdline);
	if (parse->valid) {
		execute_parsed_command(parse);
	} else {
		printf("ERROR: Command input invalid");
	}
	parse_destroy(parse);
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
	parser_init();
	print_banner();
	char *cmdline;
	while(1) {
		cmdline = readline("$ ");
		handle_exit(cmdline);
		if (strcmp(cmdline, "") != 0) {
			handle_command(cmdline);
		}
		free(cmdline);
	}
}

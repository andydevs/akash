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
#include "parse.h"

/**
 * Print banner of shell
 */
void print_banner(void) {
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

/**
 * Handle exit status of command line.
 * Exits if command line is exit.
 *
 * @param cmdline command line to check for exit status
 */
void handle_exit(char* cmdline) {
	if (!cmdline || !strcmp(cmdline, "exit")) {
		printf("Exiting...\n");
		parse_deinit();
		exit(EXIT_SUCCESS);
	}	
}

/**
 * Execute parsed command represented by parse
 * 
 * @param parse struct representing parsed command
 */
void execute_parsed_command(struct parse* parse) {
	if (parse->infile) {
		printf("Infile: %s\n", parse->infile);
	}
	if (parse->tasks) {
		printf("Tasks:\n");
		struct task_node* taskn;
		for (taskn = parse->tasks; taskn; taskn = taskn->next) {
			printf("	Task:\n");
			printf("		Command: %s\n", taskn->cmd);
			if (taskn->args) {
				printf("		Arguments:\n");
				struct arg_node* argn;
				for (argn = taskn->args; argn; argn = argn->next) {
					printf("			%s\n", argn->arg);
				}
			}
		}
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
	parse_init();
	print_banner();
	char *cmdline;
	while(1) {
		cmdline = readline("$ ");
		handle_exit(cmdline);	
		handle_command(cmdline);
		free(cmdline);
	}
}

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
#include "execute.h"
#include <stdio.h>

/**
 * Execute parsed command represented by parse
 * 
 * @param parse struct representing parsed command
 */
void execute_parsed_command(struct parse* parse) {
	if (parse->shell) {
		printf("Shell command: %s\n", parse->shcmd);
	}
	else {
		if (parse->infile) {
			printf("Infile: %s\n", parse->infile);
		}
		if (parse->outfile) {
			printf("Outfile: %s\n", parse->outfile);
		}
		printf("Background: %i\n", parse->background);
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
}

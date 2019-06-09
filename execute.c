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
#include "parser.h"
#include "execute.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

void __debug_execute__printf(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
#ifdef DEBUG_EXECUTE
	vprintf(fmt, args);
#endif
	va_end(args);
}

void execute_task_as_child(struct task_node* task) {
	__debug_execute__printf("CMD: %s\n", task->cmd);
	char* const args[2] = { task->cmd, NULL };
	pid_t pid = fork();
	switch (pid) {
		case -1:
			// ERROR
			printf("ERROR: Failed to fork!\n");
			break;
		case 0:
			// CHILD
			// Execute statement
			__debug_execute__printf("==========EXECUTE==========\n");
			execvp(task->cmd, args);
			printf("ERROR: Failed to execute!\n");
			exit(EXIT_FAILURE);
			break;
		default:
			// PARENT
			wait(NULL);	
			__debug_execute__printf("===========================\n");
			break;
	}
}

/**
 * Execute parsed command represented by parse
 * 
 * @param parse struct representing parsed command
 */
void execute_parsed_command(struct parse* parse) {
	__debug_execute__printf("================EXECUTE================\n");
	if (!parse->shell) {
		struct task_node* task = parse->tasks;
		execute_task_as_child(task);
	}
	__debug_execute__printf("=======================================\n");
}

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
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
	int error;
	char* const args[2] = { task->cmd, NULL };
	__debug_execute__printf("==========EXECUTE TASK==========\n");
	pid_t pid = fork();
	switch (pid) {
		case -1:
			// ERROR
			printf("ERROR: Failed to fork!\n");
			break;
		case 0:
			// CHILD
			// Execute statement
			error = execvp(task->cmd, args);
			if (error == -1) {
				printf("ERROR! Failed to execute %s: %s\n", task->cmd, strerror(errno));
			}
			exit(EXIT_FAILURE);
			break;
		default:
			// PARENT
			wait(NULL);	
			break;
	}
	__debug_execute__printf("===============================\n");
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

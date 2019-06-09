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

/**
 * Print on DEBUG_EXECUTE
 */
void __debug_execute__printf(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
#ifdef DEBUG_EXECUTE
	vprintf(fmt, args);
#endif
	va_end(args);
}

/**
 * Handle child process. Execute task command. Print error if error.
 *
 * @param task task node to execute
 */
void handle_child(struct task_node* task) {
	__debug_execute__printf("==========EXECUTE CHILD=========\n");
	char* const args[2] = { task->cmd, NULL };
	int error = execvp(task->cmd, args);
	if (error == -1) {
		printf("ERROR! Failed to execute %s: %s\n", task->cmd, strerror(errno));
	} 
	__debug_execute__printf("================================\n");
	exit(EXIT_FAILURE);
}

/**
 * Fork a child process and execute task in it
 * 
 * @param task task node to execute
 */
void fork_and_execute_task(struct task_node* task) {
	__debug_execute__printf("CMD: %s\n", task->cmd);
	int error;
	char* const args[2] = { task->cmd, NULL };
	__debug_execute__printf("==========EXECUTE TASK==========\n");
	pid_t pid = fork();
	switch (pid) {	
		case -1: printf("ERROR: Failed to fork!\n"); break; // ERROR
		case 0: handle_child(task); break; // CHILD	
		default: wait(NULL); break; // PARENT
	}
	__debug_execute__printf("================================\n");
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
		fork_and_execute_task(task);
	}
	__debug_execute__printf("=======================================\n");
}

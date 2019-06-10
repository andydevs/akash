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
void handle_child(struct task_node* task, char* const* args) {
	__debug_execute__printf("==========EXECUTE CHILD=========\n");
	int error = execvp(task->cmd, args);
	if (error == -1) {
		printf("ERROR! Failed to execute %s: %s\n", task->cmd, strerror(errno));
	} 
	__debug_execute__printf("================================\n");
	exit(EXIT_FAILURE);
}

/**
 * Get number of arguments from task
 *
 * @param task task node being executed
 *
 * @return number of arguments
 */
int get_number_of_arguments(struct task_node* task) {
	__debug_execute__printf("Get number of arguments: ");
	int size = 2;
	struct arg_node* argn;
	for (argn = task->args; argn; argn = argn->next) { size++; }
	__debug_execute__printf("%i\n", size);
	return size;
}

/**
 * Populate args array with arg nodes in task
 *
 * @param args args array
 * @param size size of args array
 * @param task task node to populate from
 */
void populate_args_array(char** args, int size, struct task_node* task) {
	__debug_execute__printf("Populate args array:\n");
	args[0] = task->cmd;
	args[size-1] = NULL;
	struct arg_node* argn;
	int place = size-2;
	for (argn = task->args; argn; argn = argn->next) {
		args[place] = argn->arg;
		place--;
	}
	for (int i = 0; i < size; i++) {
		if (args[i] == NULL) __debug_execute__printf("NULL\n");
		else __debug_execute__printf("%s\n", args[i]);
	}
}

/**
 * Fork a child process and execute task in it
 * 
 * @param task task node to execute
 */
void fork_and_execute_task(struct task_node* task) {
	// Begin
	__debug_execute__printf("==========FORK EXECUTE==========\n");
	__debug_execute__printf("CMD: %s\n", task->cmd);

	// Build args vector
	__debug_execute__printf("Build args vector\n");	
	int size = get_number_of_arguments(task);
	char* args[size];
	populate_args_array(args, size, task);	
	
	// Fork and execute
	__debug_execute__printf("Fork and execute...\n");
	pid_t pid = fork();
	switch (pid) {	
		case -1: printf("ERROR: Failed to fork!\n"); break;     // ERROR
		case 0:  handle_child(task, (char* const*)args); break; // CHILD	
		default: wait(NULL); break; 						    // PARENT
	}
	
	// End
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

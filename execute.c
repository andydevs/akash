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
#include <signal.h>
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

// ----------------------------- EXECUTE -------------------------------

/**
 * Handle child process. Execute task command. Print error if error.
 *
 * @param task task node to execute
 */
void handle_child(struct task_node* task, char* const* args) {
	__debug_execute__printf("Execute child:\n");

	// TODO: Set IO
	//	See Build IO todo
	
	// Execute child
	int error = execvp(task->cmd, args);
	if (error == -1) {
		printf("ERROR! Failed to execute %s: %s\n", task->cmd, strerror(errno));
	} 
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
	__debug_execute__printf("Command: %s\n", task->cmd);

	// Build args vector
	__debug_execute__printf("Build args vector\n");	
	int size = get_number_of_arguments(task);
	char* args[size];
	populate_args_array(args, size, task);	
	
	// Fork and execute
	// TODO: Send IO arguments to child
	__debug_execute__printf("Fork and execute...\n");
	pid_t pid = fork();
	switch (pid) {	
		case -1: printf("ERROR: Failed to fork!\n"); break;     // ERROR
		case 0:  handle_child(task, (char* const*)args); break; // CHILD	
		default: break;											// PARENT
	}
}

/**
 * Return number of tasks
 *
 * @param parse parse struct
 *
 * @return number of tasks
 */
int get_number_of_tasks(struct parse* parse) {
	__debug_execute__printf("Number of tasks: ");
	int size = 0;
	struct task_node* taskn;
	for (taskn = parse->tasks; taskn; taskn = taskn->next) { size++; }
	__debug_execute__printf("%i\n", size);
	return size;
}

#define IO_BUFF_SIZE 2
#define IO_READ 0
#define IO_WRITE 1

/**
 * Execute parsed command represented by parse
 * 
 * @param parse struct representing parsed command
 */
void execute_parsed_command(struct parse* parse) {
	__debug_execute__printf("================EXECUTE================\n");
	if (!parse->shell) {
		// TODO: Build IO
		//	IO is a list of file descriptor integer pairs.
		// 	Either pipes, infiles, or outfiles, arranged 
		//  as the read/write arguments for IO. If the fd
		//	int is -1, that end is kept as is in child
			
		// Create file descriptors 
		int size = get_number_of_tasks(parse);
		int pipe_fd[size-1][IO_BUFF_SIZE];
		for (int i = 0; i < size-1; i++) {
			pipe(pipe_fd[i]);
		}

		// Iterate through tasks. Fork/execute each
		int i;
		struct task_node* taskn;
		for (taskn = parse->tasks, i = 0; taskn; taskn = taskn->next, i++) {
			printf("Size %i\n", i);
			// TODO: Send IO to task through extra arguments
			fork_and_execute_task(taskn);
		}

		// Close file descriptors
		for (int i = 0; i < size; i++) {
			close(pipe_fd[i][IO_READ]);
			close(pipe_fd[i][IO_WRITE]);
		}

		// Wait for all children processes
		int pid;
		do { pid = wait(NULL); } while (pid > 0);
	}
	__debug_execute__printf("=======================================\n");
}

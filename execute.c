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

// IO Constants
#define IO_BUFF_SIZE 2
#define IO_READ 0
#define IO_WRITE 1

/**
 * Populate IO table with pipes and in/out files
 *
 * @param fd   file descriptor array
 * @param size size of file descriptor array
 */
void populate_io_table(int fd[][2], int size) {
	int pipe_fd[IO_BUFF_SIZE];
	fd[0][IO_READ] = -1;
	for (int i = 0; i < size-1; i++) {
		pipe(pipe_fd);
		fd[i][IO_WRITE] = pipe_fd[IO_WRITE];
		fd[i+1][IO_READ] = pipe_fd[IO_READ];
	}
	fd[size-1][IO_WRITE] = -1;
}

/**
 * Close all open files in IO table
 *
 * @param fd   file descriptor array
 * @param size size of file descriptor array
 */
void close_io_table(int fd[][2], int size) {
	for (int i = 0; i < size; i++) {
		if (fd[i][IO_READ] != -1) {
			close(fd[i][IO_READ]);
		}
		if (fd[i][IO_WRITE] != -1) {
			close(fd[i][IO_WRITE]);
		}
	}
}

// ----------------------------- EXECUTE -------------------------------

/**
 * Handle child process. Execute task command. Print error if error.
 *
 * @param task task node to execute
 * @param args args list
 * @param ind  task index number
 * @param fd   file descriptor table
 * @param size size of file descriptor table
 */
void handle_child(struct task_node* task, char* const* args, int ind, int fd[][2], int size) {
	__debug_execute__printf("Execute child:\n");

	// TODO: Set IO
	if (fd[ind][IO_READ] != -1) {
		dup2(fd[ind][IO_READ], STDIN_FILENO);
	}
	if (fd[ind][IO_WRITE] != -1) {
		dup2(fd[ind][IO_WRITE], STDOUT_FILENO);
	}
	close_io_table(fd, size);	
	
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
 * @param args args list
 *
 * @return number of arguments
 */
int get_number_of_arguments(struct arg_node* args) {
	__debug_execute__printf("Get number of arguments: ");
	int size = 2;
	struct arg_node* argn;
	for (argn = args; argn; argn = argn->next) { size++; }
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
 * @param ind  index of task node
 * @param fd   file descriptor table
 * @param size size of file descriptor table
 */
void fork_and_execute_task(struct task_node* task, int ind, int fd[][2], int size) {
	// Begin
	__debug_execute__printf("Index: %i\n", ind);
	__debug_execute__printf("Command: %s\n", task->cmd);

	// Build args vector
	__debug_execute__printf("Build args vector\n");	
	int asize = get_number_of_arguments(task->args);
	char* args[asize];
	populate_args_array(args, asize, task);	
	
	// Fork and execute
	// TODO: Send IO arguments to child
	__debug_execute__printf("Fork and execute...\n");
	pid_t pid = fork();
	switch (pid) {
		// ERROR
		case -1: printf("ERROR: Failed to fork!\n"); break; 
		// CHILD
		case 0:  handle_child(task, (char* const*)args, ind, fd, size); break; 
		// PARENT
		default: break; 
	}
}

/**
 * Return number of tasks
 *
 * @param tasks tasks list
 *
 * @return number of tasks
 */
int get_number_of_tasks(struct task_node* tasks) {
	__debug_execute__printf("Number of tasks: ");
	int size = 0;
	struct task_node* taskn;
	for (taskn = tasks; taskn; taskn = taskn->next) { size++; }
	__debug_execute__printf("%i\n", size);
	return size;
}

/**
 * Reverse tasks linked list
 *
 * @param tasks task list pointer
 */
struct task_node* reverse_tasks_list(struct task_node* tasks) {
	struct task_node *taskp, *taskc, *taskn;
	taskc = tasks;
	taskp = NULL;
	taskn = NULL;
	while (taskc != NULL) {
		taskn = taskc->next;
		taskc->next = taskp;
		taskp = taskc;
		taskc = taskn;
	}
	return taskp;
}

/**
 * Execute parsed command represented by parse
 * 
 * @param parse struct representing parsed command
 */
void execute_parsed_command(struct parse* parse) {
	__debug_execute__printf("================EXECUTE================\n");
	if (!parse->shell) {
		// Get number of tasks	
		int size = get_number_of_tasks(parse->tasks);

		// Build IO
		// IO is a list of file descriptor integer pairs.
		// Either pipes, infiles, or outfiles, arranged 
		// as the read/write arguments for IO. If the fd
		// int is -1, that end is kept as is in child
		int fd[size][IO_BUFF_SIZE];
		populate_io_table(fd, size);		

		// Reverse tasks list
		struct task_node* reversed = reverse_tasks_list(parse->tasks);

		// Iterate through tasks. Fork/execute each
		int ind;
		struct task_node* taskn;
		for (taskn = reversed, ind = 0; taskn; taskn = taskn->next, ind++) {
			fork_and_execute_task(taskn, ind, fd, size);
		}

		// Close file descriptors
		close_io_table(fd, size);	

		// Wait for all children processes
		int pid;
		do { pid = wait(NULL); } while (pid > 0);
	}
	__debug_execute__printf("=======================================\n");
}

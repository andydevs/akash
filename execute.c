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
#include "debug.h"
#include "execute.h"
#include "parser.h"
#include "taskslist.h"
#include "argslist.h"
#include "io.h"
#include "shellcommand.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

// Debug system
#ifdef DEBUG_EXECUTE_EXECUTE
DEBUG_ON("\e[34m[execute:execute]\e[0m")
#else
DEBUG_OFF
#endif

/**
 * Execute task command in child process. Print error if error.
 *
 * @param task task node to execute
 * @param args args list
 */
void execute_task_in_child(struct task_node* task, char* const* args) {
	__debug__printf("Execute task in child:\n");
	// Execute child
	int error = execvp(task->cmd, args);
	if (error == -1) {
		printf("ERROR! Failed to execute %s: %s\n", task->cmd, strerror(errno));
	} 
	exit(EXIT_FAILURE);
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
	__debug__printf("Index: %i\n", ind);
	__debug__printf("Command: %s\n", task->cmd);

	// Build args vector
	__debug__printf("Build args vector\n");	
	int asize = get_number_of_arguments(task->args);
	char* args[asize];
	populate_args_array(args, asize, task);	
	
	// Fork and execute
	__debug__printf("Fork and execute...\n");
	pid_t pid = fork();
	switch (pid) {
		// ERROR
		case -1: printf("ERROR: Failed to fork!\n"); break; 
		// CHILD
		case 0:
			set_io_in_child(ind, fd, size);	
			execute_task_in_child(task, (char* const*)args);
			break; 
		// PARENT
		default: break; 
	}
}

/**
 * Execute parsed command represented by parse
 * 
 * @param parse struct representing parsed command
 */
void execute_parsed_command(struct parse* parse) {
	if (parse->shell) {
		// Parse shell command
		__debug__printf("Process shell command: %s\n", parse->shcmd);
		execute_shellcommand(parse->shcmd);		
	}
	else {
		// Get number of tasks	
		int size = get_number_of_tasks(parse->tasks);

		// Build IO
		// IO is a list of file descriptor integer pairs.
		// Either pipes, infiles, or outfiles, arranged 
		// as the read/write arguments for IO. If the fd
		// int is -1, that end is kept as is in child
		int fd[size][IO_BUFF_SIZE];
		populate_io_table(fd, size, parse->infile, parse->outfile);

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
}

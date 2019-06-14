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
#include "io.h"
#include "debug.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// Debug system
#ifdef DEBUG_EXECUTE_ARGSLIST
DEBUG_ON("\e[33m[execute:io]\e[0m")
#else
DEBUG_OFF
#endif

/**
 * Populate IO table with pipes and in/out files
 *
 * @param fd      file descriptor array
 * @param size    size of file descriptor array
 * @param infile  name of infile (NULL if none specified)
 * @param outfile name of outfile (NULL if none specified)
 */
void populate_io_table(int fd[][IO_BUFF_SIZE], int size, char* infile, char* outfile) {
	// Handle creating infile
	if (infile) {
		fd[0][IO_READ] = open(infile, O_RDONLY);
	}
	else {
		fd[0][IO_READ] = -1;
	}

	// Handle creating pipes
	int pipe_fd[IO_BUFF_SIZE];
	for (int i = 0; i < size-1; i++) {
		pipe(pipe_fd);
		fd[i][IO_WRITE] = pipe_fd[IO_WRITE];
		fd[i+1][IO_READ] = pipe_fd[IO_READ];
	}
	
	// Handle creating outfile
	if (outfile) {
		fd[size-1][IO_WRITE] = open(outfile, O_WRONLY | O_CREAT, IO_STDMODE);
	}
	else {
		fd[size-1][IO_WRITE] = -1;
	}
}

/**
 * Close all open files in IO table
 *
 * @param fd   file descriptor array
 * @param size size of file descriptor array
 */
void close_io_table(int fd[][IO_BUFF_SIZE], int size) {
	for (int i = 0; i < size; i++) {
		if (fd[i][IO_READ] != -1) {
			close(fd[i][IO_READ]);
		}
		if (fd[i][IO_WRITE] != -1) {
			close(fd[i][IO_WRITE]);
		}
	}
}

/**
 * Set IO file descriptors in child process
 * 
 * @param ind  index of task for child process
 * @param fd   file descriptor table
 * @param size size of file descriptor table
 */
void set_io_in_child(int ind, int fd[][IO_BUFF_SIZE], int size) {
	__debug__printf("Set IO in child:\n");
	if (fd[ind][IO_READ] != -1) {
		dup2(fd[ind][IO_READ], STDIN_FILENO);
	}
	if (fd[ind][IO_WRITE] != -1) {
		dup2(fd[ind][IO_WRITE], STDOUT_FILENO);
	}
	close_io_table(fd, size);	
}

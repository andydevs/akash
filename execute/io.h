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
#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// IO Constants
#define IO_BUFF_SIZE 2
#define IO_READ 0
#define IO_WRITE 1
#define IO_STDMODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH

/**
 * Populate IO table with pipes and in/out files
 *
 * @param fd      file descriptor array
 * @param size    size of file descriptor array
 * @param infile  name of infile (NULL if none specified)
 * @param outfile name of outfile (NULL if none specified)
 */
void populate_io_table(int fd[][IO_BUFF_SIZE], int size, char* infile, char* outfile);

/**
 * Close all open files in IO table
 *
 * @param fd   file descriptor array
 * @param size size of file descriptor array
 */
void close_io_table(int fd[][IO_BUFF_SIZE], int size);

/**
 * Set IO file descriptors in child process
 * 
 * @param ind  index of task for child process
 * @param fd   file descriptor table
 * @param size size of file descriptor table
 */
void set_io_in_child(int ind, int fd[][IO_BUFF_SIZE], int size);

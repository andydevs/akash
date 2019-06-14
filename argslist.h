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
#include "parse.h"

/**
 * Get number of arguments from task
 *
 * @param args args list
 *
 * @return number of arguments
 */
int get_number_of_arguments(struct arg_node* args);

/**
 * Populate args array with arg nodes in task
 *
 * @param args args array
 * @param size size of args array
 * @param task task node to populate from
 */
void populate_args_array(char** args, int size, struct task_node* task);

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
#include <stdlib.h>

/**
 * Destroy args linked list
 * 
 * @param args args linked list
 */
void parse_args_destroy(struct arg_node* args) {
	struct arg_node* current = args;
	struct arg_node* next;
	while (current) {
		next = current->next;
		free(current);
		current = next;
	}
}

/**
 * Destroy task struct
 * 
 * @param task task struct
 */
void parse_task_destroy(struct task* task) {
	if (task->cmd) free(task->cmd);
	if (task->args) parse_args_destroy(task->args);
	free(task);
}

/**
 * Destroy parse struct
 *
 * @param parse struct
 */
void parse_destroy(struct parse* parse) {
	if (parse->task) parse_task_destroy(parse->task);
	free(parse);
}

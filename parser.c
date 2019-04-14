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
 * Create new task node
 *
 * @return new created task node
 */
struct task_node* task_new() {
	struct task_node* task = NEW(struct task_node);
	task->next = NULL;
	return task;	
}

/**
 * Add arg to task
 *
 * @parse task task struct
 * @parse arg  argument string
 */
void parse_append_task(struct parse* parse, struct task_node* taskn) {
	// Set tasks linked list to task 
	// node if linked list has no nodes
	if (parse->tasks == NULL) {
		parse->tasks = taskn;
	}
	else {
		// Else loop to the end of the list
		// and append the node there
		struct task_node* taski = parse->tasks;
		while (taski->next) {
			taski = taski->next;
		}
		taski->next = taskn;
	}
}

/**
 * Add arg to task
 *
 * @parse task task struct
 * @parse arg  argument string
 */
void task_prepend_arg(struct task_node* task, char* arg) {
	struct arg_node* argn = NEW(struct arg_node);
	argn->arg  = arg;
	argn->next = task->args;
	task->args = argn;	
}

// ---------------------------- DESTROY ----------------------------

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
void parse_task_destroy(struct task_node* task) {
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
	if (parse->tasks) parse_task_destroy(parse->tasks);
	free(parse);
}

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
 * Create new parse
 *
 * @return new created parse
 */
struct parse* parse_new() {
	struct parse* parse = NEW(struct parse);
	parse->shell = 0;
	parse->shcmd = NULL;
	parse->tasks = NULL;
	parse->infile = NULL;
	parse->outfile = NULL;
	parse->background = 0;
	return parse;
}

/**
 * Set shell command of parse
 *
 * @param parse parse struct
 * @param shcmd shell command name 
 */
void parse_set_shcmd(struct parse* parse, char* shcmd) {
	parse->shell = 1;
	parse->shcmd = shcmd;
}

/**
 * Set infile of parse
 * 
 * @param parse  parse struct
 * @param infile infile
 */
void parse_set_infile(struct parse* parse, char* infile) {
	parse->infile = infile;
}

/**
 * Set outfile of parse
 * 
 * @param parse   parse struct
 * @param outfile outfile
 */
void parse_set_outfile(struct parse* parse, char* outfile) {
	parse->outfile = outfile;	
}

/**
 * Set background of parse
 * 
 * @param parse   parse struct
 * @param outfile background
 */
void parse_set_background(struct parse* parse, int background) {
	parse->background = background;
}

/**
 * Create new task node
 *
 * @return new created task node
 */
struct task_node* task_new() {
	struct task_node* task = NEW(struct task_node);
	task->next = NULL;
	task->args = NULL;
	return task;	
}

/**
 * Add task to parse
 *
 * @parse parse parse struct
 * @parse task  task struct
 */
void parse_prepend_task(struct parse* parse, struct task_node* taskn) {
	taskn->next = parse->tasks;
	parse->tasks = taskn;
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
	if (parse->infile) free(parse->infile);
	if (parse->outfile) free(parse->outfile);
	if (parse->shcmd) free(parse->shcmd);
	if (parse->tasks) parse_task_destroy(parse->tasks);
	free(parse);
}

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

#define NEW(type) (type*)malloc(sizeof(type))

struct arg_node {
	char* arg;
	struct arg_node* next;
};

struct task_node {
	char* cmd;
	struct arg_node* args;
	struct task_node* next;
};

struct parse {
	int valid;
	char* infile;
	struct task_node* tasks;
};

/**
 * Create new parse
 *
 * @return new created parse
 */
struct parse* parse_new();

/**
 * Set infile of parse
 * 
 * @param parse  parse struct
 * @param infile infile
 */
void parse_set_infile(struct parse* parse, char* infile);

/**
 * Create new task node
 *
 * @return new created task node
 */
struct task_node* task_new();
 
/**
 * Add task to parse
 *
 * @parse parse parse struct
 * @parse task  task struct
 */
void parse_prepend_task(struct parse* parse, struct task_node* taskn);

/**
 * Add arg to task
 *
 * @parse task task struct
 * @parse arg  argument string
 */
void task_prepend_arg(struct task_node* task, char* arg);

/**
 * Destroy parse struct
 *
 * @param parse struct
 */
void parse_destroy(struct parse* parse);

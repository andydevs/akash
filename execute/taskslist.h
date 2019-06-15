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
#include "parser/parse.h"

/**
 * Return number of tasks
 *
 * @param tasks tasks list
 *
 * @return number of tasks
 */
int get_number_of_tasks(struct task_node* tasks);

/**
 * Reverse tasks linked list
 *
 * @param tasks task list pointer
 *
 * @return reversed task list pointer
 */
struct task_node* reverse_tasks_list(struct task_node* tasks);

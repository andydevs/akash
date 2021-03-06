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
#include "taskslist.h"
#include "debug.h"

// Debug system
#ifdef DEBUG_EXECUTE_TASKSLIST
DEBUG_ON("\e[32m[execute:taskslist]\e[0m")
#else
DEBUG_OFF
#endif

/**
 * Return number of tasks
 *
 * @param tasks tasks list
 *
 * @return number of tasks
 */
int get_number_of_tasks(struct task_node* tasks) {
	__debug__printf("Number of tasks:\n");
	int size = 0;
	struct task_node* taskn;
	for (taskn = tasks; taskn; taskn = taskn->next) { size++; }
	__debug__printf("%i\n", size);
	return size;
}

/**
 * Reverse tasks linked list
 *
 * @param tasks task list pointer
 *
 * @return reversed task list pointer
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

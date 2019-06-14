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
#include "argslist.h"
#include <stdarg.h>
#include <stdio.h>

/**
 * Print on DEBUG_EXECUTE_ARGSLIST
 */
static void __debug__printf(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
#ifdef DEBUG_EXECUTE_ARGSLIST
	vprintf(fmt, args);
#endif
	va_end(args);
}


/**
 * Get number of arguments from task
 *
 * @param args args list
 *
 * @return number of arguments
 */
int get_number_of_arguments(struct arg_node* args) {
	__debug__printf("Get number of arguments: ");
	int size = 2;
	struct arg_node* argn;
	for (argn = args; argn; argn = argn->next) { size++; }
	__debug__printf("%i\n", size);
	return size;
}

/**
 * Populate args array with arg nodes in task
 *
 * @param args args array
 * @param size size of args array
 * @param task task node to populate from
 */
void populate_args_array(char** args, int size, struct task_node* task) {
	__debug__printf("Populate args array:\n");
	args[0] = task->cmd;
	args[size-1] = NULL;
	struct arg_node* argn;
	int place = size-2;
	for (argn = task->args; argn; argn = argn->next) {
		args[place] = argn->arg;
		place--;
	}
	for (int i = 0; i < size; i++) {
		if (args[i] == NULL) __debug__printf("NULL\n");
		else __debug__printf("%s\n", args[i]);
	}
}


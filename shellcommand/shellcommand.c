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
#include "shellcommand.h"
#include "debug.h"
#include <string.h>
#include <stdio.h>

// Debug statements
#ifdef DEBUG_SHELLCOMMAND
DEBUG_ON("[shellcommand]")
#else
DEBUG_OFF
#endif

/**
 * Print who is the shell
 */
void whoami() {
	printf("I am akash!\n");
}

/**
 * Shell command not found
 */
void shellnotfound(char* shcmd) {
	printf("ERROR! Shell command not found: %s\n", shcmd);
}

/**
 * Lookup and execute shellcommand
 *
 * @param shcmd shell command name to execute
 */
void execute_shellcommand(char* shcmd) {
	if (strcmp(shcmd, "whoami")) whoami();
	else shellnotfound(shcmd);
}

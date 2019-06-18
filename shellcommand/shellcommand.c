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
#include "version.h"
#include <string.h>
#include <stdio.h>

// Debug statements
#ifdef DEBUG_SHELLSHCMD
DEBUG_ON("\e[93m[shellcommand]\e[0m")
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
 * Print shell version
 */
void version() {
	printf("v%i.%i.%i\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
}

/**
 * Print author information
 */
void author() {
	printf("Author: %s\n", AUTHOR_NAME);
	printf("Github: %s\n", AUTHOR_GITHUB);
	printf("Email: %s\n", AUTHOR_EMAIL);
}

// ------------------------------------ EXECUTE ------------------------------------

// Shell command handling macro
#define SHCMD(command, func) \
	__debug__printf("Recieved command \"%s\"\n", shcmd); \
	__debug__printf("Compare with \"%s\": %i\n", command, strcmp(shcmd, command)); \
	__debug__printf("Matches \"%s\": %i\n", command, strcmp(shcmd, command) == 0); \
	if (strcmp(shcmd, command) == 0) { func(); return; }

/**
 * Lookup and execute shellcommand
 *
 * @param shcmd shell command name to execute
 */
void execute_shellcommand(char* shcmd) {
	// Shell command handling
	SHCMD("whoami", whoami);
	SHCMD("version", version);
	SHCMD("author", author);

	// Shell command not found
	__debug__printf("No matches for \"%s\"\n", shcmd);
	printf("ERROR! Shell command not found: %s\n", shcmd);
}

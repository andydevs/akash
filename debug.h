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
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define DEBUG_ON(HEADER) \
	static void __debug__printf(const char* fmt, ...) { \
		char fullfmt[128]; \
		strcpy(fullfmt, HEADER); \
		strcat(fullfmt, " "); \
		strcat(fullfmt, fmt); \
		va_list args; \
		va_start(args, fmt); \
		vprintf(fullfmt, args); \
		va_end(args); \
	};

#define DEBUG_OFF \
	static void __debug__printf(const char* fmt, ...) { };

#ifdef DEBUG
#define DEBUG_PARSE
#define DEBUG_EXECUTE
#endif

#ifdef DEBUG_PARSE
#define DEBUG_PARSE_PARSE
#define DEBUG_PARSE_PARSER
#define DEBUG_PARSE_TOKENIZER
#endif

#ifdef DEBUG_EXECUTE
#define DEBUG_EXECUTE_EXECUTE
#define DEBUG_EXECUTE_IO
#define DEBUG_EXECUTE_ARGSLIST
#define DEBUG_EXECUTE_TASKSLIST
#endif

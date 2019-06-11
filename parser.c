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
#include "tokenizer.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/**
 * DEBUG_PARSE print statement
 *
 * @param fmt output format
 * @param ... varargs to apply to output
 */
void __debug_parse__printf(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
#ifdef DEBUG_PARSE
	vprintf(fmt, args);
#endif
	va_end(args);
}

// -------------------- PARSE TEMPLATE MACROS ---------------------

#define PARSE_INVALID() \
	parse->valid = 0; \
	__debug_parse__printf("Command line invalid\n"); \
	__debug_parse__printf("=========================\n\n"); \
	return parse;

#define PARSE_REQUIRE(parse_result) \
	if (parse_result) { PARSE_INVALID() }	

#define PARSE_OPTIONAL(parse_result) \
	int result = parse_result; \
	if (!(result == 0 || result == REG_NOMATCH)) { PARSE_INVALID() }

#define PART_REQUIRE(part_result) \
	if (part_result) { return 1; }

#define PART_OPTIONAL(part_result) \
	int result = part_result; \
	if (!(result == 0 || result == REG_NOMATCH)) { return 1; }

// -------------------------- PARSE NODES ---------------------------

/**
 * Parse shell command
 *
 * @param parse   parse struct
 * @param cmdline command input line
 * @param index   starting index of input to check (updates)
 *
 * @return regexec result (0 if match is successful)
 */
int parse_shcmd(struct parse* parse, char* cmdline, int* index) {
	char* cmdname;
	int error = consume(&shcmd, cmdline, index, &cmdname);
	if (!error) {
		parse_set_shcmd(parse, cmdname);
	}
	return error;
}

/**
 * Parse command
 *
 * @param task    task struct
 * @param cmdline command input line
 * @param index   starting index of input to check (updates)
 *
 * @return regexec result (0 if match is successful)
 */
int parse_cmd(struct task_node* task, char* cmdline, int* index) {
	char* filename;
	int error = consume(&file, cmdline, index, &filename);
	if (!error) {
		task->cmd = filename;
		__debug_parse__printf("COMMAND: %s\n", task->cmd);
	}
	return error;
}

/**
 * Parse argument
 *
 * @param task    task struct
 * @param cmdline command input line
 * @param index   starting index of input to check (updates)
 *
 * @return regexec result (0 if match is successful)
 */
int parse_arg(struct task_node* task, char* cmdline, int* index) {
	char* argstring;
	int error = consume(&arg, cmdline, index, &argstring);
	if (!error) {
		// Add argument to parse
		task_prepend_arg(task, argstring); 
		__debug_parse__printf("ARGUMENT: %s\n", task->args->arg);
	}
	return error;
}

/**
 * Parse all arguments
 *
 * @param task    task struct
 * @param cmdline command input line
 * @param index   starting index of input to check (updates)
 *
 * @return regexec result (0 if match is successful)
 */
int parse_args(struct task_node* task, char* cmdline, int* index) {
	task->args = NULL;
	int last = 0;
	while (!(last || lookahead(&arg, cmdline, index))) {
		last = parse_arg(task, cmdline, index);
	}
	return last;
}

/**
 * Parse task
 *
 * @param parse   parse struct
 * @parse cmdline command input line
 * @param index   starting index of input to check (updates)
 *
 * @return parse result (0 if match is successful)
 */
int parse_task(struct parse* parse, char* cmdline, int* index) {
	struct task_node* task = NEW(struct task_node);
	__debug_parse__printf("TASK\n");
	PART_REQUIRE(parse_cmd(task, cmdline, index))
	PART_OPTIONAL(parse_args(task, cmdline, index))
	parse_prepend_task(parse, task);
	return 0;
}

/**
 * Parse in file
 *
 * @param parse   parse struct
 * @param cmdline command input line
 * @param index   starting index of input to check (updates)
 *
 * @return regexec result (0 if match is successful)
 */
int parse_infile(struct parse* parse, char* cmdline, int* index) {
	char* filename;
	int error = consume(&file, cmdline, index, &filename);
	if (!error) {
		// Set infile
		parse_set_infile(parse, filename);
		__debug_parse__printf("INFILE: %s\n", parse->infile);	
	}
	return error;
}

/**
 * Parse out file
 *
 * @param parse   parse struct
 * @param cmdline command input line
 * @param index   starting index of input to check (updates)
 *
 * @return regexec result (0 if match is successful)
 */
int parse_outfile(struct parse* parse, char* cmdline, int* index) {
	char* filename;
	int error = consume(&file, cmdline, index, &filename);
	if (!error) {
		// Add command to parse
		parse_set_outfile(parse, filename);
		__debug_parse__printf("OUTFILE: %s\n", parse->outfile);	
	}
	return error;
}

// --------------------- MAIN PARSE FUNCTION ----------------------

/**
 * Initialize parser
 */
void parser_init() {
	tokenizer_init();
}

/**
 * Deinitialize parser
 */
void parser_deinit() {
	tokenizer_deinit();
}

/**
 * Parse command input into parse struct
 * 
 * @param cmdline command line input
 *
 * @return parse struct
 */
struct parse* parse_command_input(char* cmdline) {
	// Parse header entered
	__debug_parse__printf("\n==========PARSE==========\n"); \
	__debug_parse__printf("Entered: %s\n", cmdline);
	
	// Declare index int and allocate parse struct memory
	int index = 0;
	struct parse* parse = parse_new();

	// If shell command
	if (consume(&shell, cmdline, &index, NULL) == 0) {
		PARSE_REQUIRE(parse_shcmd(parse, cmdline, &index));
	}
	else {
		// First task
		PARSE_REQUIRE(parse_task(parse, cmdline, &index));
		// In-file
		if (consume(&file_in, cmdline, &index, NULL) == 0) {
			PARSE_REQUIRE(parse_infile(parse, cmdline, &index));
		}
		// Piped tasks
		while (consume(&piper, cmdline, &index, NULL) == 0) {
			PARSE_OPTIONAL(parse_task(parse, cmdline, &index));
		}
		// Out-file
		if (consume(&file_out, cmdline, &index, NULL) == 0) {
			PARSE_REQUIRE(parse_outfile(parse, cmdline, &index));
		}
		// Background
		if (consume(&background, cmdline, &index, NULL) == 0) {
			parse_set_background(parse, 1);
		}
	}

	// Exit parse valid
	parse->valid = 1; 
	__debug_parse__printf("Command line valid\n"); 
	__debug_parse__printf("=========================\n\n"); 
	return parse;
}

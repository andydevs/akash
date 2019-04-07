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
#include "parse.h"
#include <regex.h>
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

#define NEW(type) (type*)malloc(sizeof(type))

#define PARSE_INVALID() \
	parse->valid = 0; \
	__debug_parse__printf("Command line invalid\n"); \
	__debug_parse__printf("=========================\n\n"); \
	return parse;


#define PARSE_VALID() \
	parse->valid = 1; \
	__debug_parse__printf("Command line valid\n"); \
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

// Regex strings
const char* file_string = "(\\w|[\\.-~])+(/(\\w|[\\.-])+)*";
const char* arg_string = "[^ \t\\|]+";

// Regular Expressions
regex_t file;
regex_t arg;

/**
 * Initialize parser
 */
void parse_init() {
	regcomp(&file, file_string, REG_EXTENDED);
	regcomp(&arg, arg_string, REG_EXTENDED);
}

/**
 * Deinitialize parser
 */
void parse_deinit() {
	regfree(&file);
	regfree(&arg);
}

/**
 * Print parser error for regex
 *
 * @param error error code
 * @param regex regex causing error
 */
void print_parse_error(int error, regex_t* regex) {
	size_t errbuf_size = regerror(error, regex, (char*)NULL, (size_t)0);
	char* errbuf = (char*)malloc(errbuf_size*sizeof(char));
	regerror(error, regex, errbuf, errbuf_size);
	printf("ERROR: %s\n", errbuf);
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
int parse_cmd(struct task* task, char* cmdline, int* index) {
	// Find match in string at index
	regmatch_t match[1];
	int error = regexec(&file, &cmdline[*index], 1, match, 0);
	switch (error) {
		case 0:
			// Add command to parse
			task->cmd = strndup(
				&cmdline[*index + match[0].rm_so],
				match[0].rm_eo - match[0].rm_so);
			__debug_parse__printf("\tCommand: %s\n", task->cmd);	
		
			// Add end offset to index
			*index += match[0].rm_eo;
			break;
		case REG_NOMATCH:
			// Handle no parse
			task->cmd = NULL;
			break;
		default:
			// Handle parse error
			task->cmd = NULL;
			print_parse_error(error, &file);
			break;
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
int parse_arg(struct task* task, char* cmdline, int* index) {
	// Find match in string at index
	regmatch_t match[1];
	int error = regexec(&arg, &cmdline[*index], 1, match, 0);
	switch (error) {
		case 0:
			;
			// Add argument to parse
			struct arg_node* argn = NEW(struct arg_node);
			argn->text = strndup(
				&cmdline[*index + match[0].rm_so],
				match[0].rm_eo - match[0].rm_so);
			argn->next = task->args;
			task->args = argn;
			__debug_parse__printf("\tArgument: %s\n", argn->text);	
			
			// Add end offset to index
			*index += match[0].rm_eo;
			break;
		case REG_NOMATCH:
			break;
		default:
			// Handle parse error
			print_parse_error(error, &arg);
			break;
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
int parse_args(struct task* task, char* cmdline, int* index) {
	task->args = NULL;
	int last = 0;
	while (last == 0) {
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
	struct task* task = NEW(struct task);
	__debug_parse__printf("Task:\n");
	PART_REQUIRE(parse_cmd(task, cmdline, index))
	PART_OPTIONAL(parse_args(task, cmdline, index))
	parse->task = task;
	return 0;
}

/**
 * Parse command input into parse struct
 * 
 * @param cmdline command line input
 *
 * @return parse struct
 */
struct parse* parse_command_input(char* cmdline) {
	__debug_parse__printf("\n==========PARSE==========\n");
	__debug_parse__printf("Entered: %s\n", cmdline);	
	
	// Declare index int and allocate parse struct memory
	int index = 0;
	struct parse* parse = NEW(struct parse);
	
	// Parse component
	PARSE_REQUIRE(parse_task(parse, cmdline, &index))	

	// Exit valid
	PARSE_VALID()
}

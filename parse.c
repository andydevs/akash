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

// Regex strings
const char* file_string = "(\\w|[\\.-~])+(/(\\w|[\\.-])+)*";
const char* arg_string = "\\S+";

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
 * @param parse   parse struct
 * @param cmdline command input line
 * @param index   starting index of input to check (updates)
 *
 * @return regexec result (0 if match is successful)
 */
int parse_cmd(struct parse* parse, char* cmdline, int* index) {
	// Find match in string at index
	regmatch_t match[1];
	int error = regexec(&file, &cmdline[*index], 1, match, 0);
	if (error) {
		// Handle parse error
		parse->cmd = NULL;
		print_parse_error(error, &file);
	}
	else {
		// Add command to parse
		parse->cmd = strndup(&cmdline[*index + match[0].rm_so], match[0].rm_eo - match[0].rm_so);
		__debug_parse__printf("Command: %s\n", parse->cmd);	
		
		// Add end offset to index
		*index += match[0].rm_eo;
	}
	return error;
}

/**
 * Parse argument
 *
 * @param parse   parse struct
 * @param cmdline command input line
 * @param index   starting index of input to check (updates)
 *
 * @return regexec result (0 if match is successful)
 */
int parse_arg(struct parse* parse, char* cmdline, int* index) {
	// Find match in string at index
	regmatch_t match[1];
	int error = regexec(&arg, &cmdline[*index], 1, match, 0);
	if (error) {
		// Handle parse error
		parse->arg = NULL;
		print_parse_error(error, &arg);
	}
	else {
		// Add argument to parse
		parse->arg = strndup(&cmdline[*index + match[0].rm_so], match[0].rm_eo - match[0].rm_so);
		__debug_parse__printf("Argument: %s\n", parse->arg);	
		
		// Add end offset to index
		*index += match[0].rm_eo;
	}
	return error;
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
	struct parse* parse = (struct parse*)malloc(sizeof(struct parse));
	
	// Parse command
	// Exit invalid if index is less than 0
	if (parse_cmd(parse, cmdline, &index)) {
		parse->valid = 0;
		__debug_parse__printf("Command line invalid\n");
		__debug_parse__printf("=========================\n\n");
		return parse;
	}
	__debug_parse__printf("Remaining: %s\n", &cmdline[index]);

	// Parse arg
	// Exit invalid if index is less than 0
	if (parse_arg(parse, cmdline, &index)) {	
		parse->valid = 0;
		__debug_parse__printf("Command line invalid\n");
		__debug_parse__printf("=========================\n\n");
		return parse;
	}
	__debug_parse__printf("Remaining: %s\n", &cmdline[index]);
	
	// Exit valid
	parse->valid = 1;
	__debug_parse__printf("Command line valid\n");
	__debug_parse__printf("=========================\n\n");
	return parse;
}

void parse_destroy(struct parse** parse) {
	if ((*parse)->cmd) free((*parse)->cmd);
	if ((*parse)->arg) free((*parse)->arg);
	free(*parse);
	*parse = NULL;
}

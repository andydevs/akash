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

// -------------------- PARSE TEMPLATE MACROS ---------------------

#define PARSE_HEADER() \
	__debug_parse__printf("\n==========PARSE==========\n"); \
	__debug_parse__printf("Entered: %s\n", cmdline);

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

// ----------------------- SETTING UP PARSER ------------------------

// Regex strings
const char* file_string = "^\\s*(\\w|[\\.-~])+(/(\\w|[\\.-])+)*";
const char* arg_string = "^\\s*[^ \t\\|<>]+";
const char* pipe_string = "^\\s*\\|";
const char* file_in_string = "^\\s*<";
const char* file_out_string = "^\\s*>";
const char* whitespace_string = "^\\s*";

// Regular Expressions
regex_t file;
regex_t arg;
regex_t pipe;
regex_t file_in;
regex_t file_out;
regex_t whitespace;

/**
 * Initialize parser
 */
void parse_init() {
	regcomp(&file, file_string, REG_EXTENDED);
	regcomp(&arg, arg_string, REG_EXTENDED);
	regcomp(&pipe, pipe_string, REG_EXTENDED);
	regcomp(&file_in, file_in_string, REG_EXTENDED);
	regcomp(&file_out, file_out_string, REG_EXTENDED);
	regcomp(&whitespace, whitespace_string, REG_EXTENDED);
}

/**
 * Deinitialize parser
 */
void parse_deinit() {
	regfree(&file);
	regfree(&arg);
	regfree(&pipe);
	regfree(&file_in);
	regfree(&file_out);
	regfree(&whitespace);
}

// -------------------------- PARSE HELPERS ---------------------------

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
 * Get match string from cmdline using match array
 * 
 * @param cmdline cmdline string
 * @param index   index of match
 * @param match   match offset buffer
 *
 * @return matched string
 */
char* get_match_string(char* cmdline, int* index, regmatch_t match[1]) {
	int start = match[0].rm_so;
	int end = match[0].rm_eo;
	regmatch_t wmatch[1];
	int error = regexec(&whitespace, &cmdline[*index + start], 1, wmatch, 0);
	if (error) {
		print_parse_error(error, &whitespace);
	} else {
		start += wmatch[0].rm_eo;
	}	
	return strndup(&cmdline[*index + start], end - start);
}

// -------------------------- TOKEN PARSE ----------------------------

/**
 * Consume token
 * 
 * @param token   regex token
 * @param cmdline command input line
 * @param index   starting index of input to check (updates)
 * @param output  output buffer to set to token value
 *
 * @return true if token was consumed
 */
int consume(regex_t* token, char* cmdline, int* index, char** output) {
	regmatch_t match[1];
	int error = regexec(token, &cmdline[*index], 1, match, 0);
	switch (error) {
		case 0:
			// Set output if given
			if (output) {
				*output = get_match_string(cmdline, index, match);
			}

			// Add end offset to index
			*index += match[0].rm_eo;
			break;
		case REG_NOMATCH:
			break;
		default:
			// Handle parse error
			print_parse_error(error, token);
			break;
	}
	return error;
}

/**
 * Lookahead for next token
 *
 * @param token   token regex
 * @param cmdline command input line
 * @param index   starting index of input to check (does not update)
 *
 * @return lookahead result
 */
int lookahead(regex_t* token, char* cmdline, int* index) {
	return regexec(&arg, &cmdline[*index], 0, NULL, 0);
}


// -------------------------- PARSE NODES ---------------------------

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

/**
 * Parse tasks
 *
 * @param parse   parse struct
 * @param cmdline command input line
 * @param index   starting index of input to check (updates)
 *
 * @return parse result (0 if match is successful)
 */
int parse_tasks(struct parse* parse, char* cmdline, int* index) {	
	__debug_parse__printf("TASKS\n");
	PART_REQUIRE(parse_task(parse, cmdline, index));
	if (consume(&file_in, cmdline, index, NULL) == 0) {
		PART_REQUIRE(parse_infile(parse, cmdline, index));
	}
	while (consume(&pipe, cmdline, index, NULL) == 0) {
		PART_OPTIONAL(parse_task(parse, cmdline, index));
	}
	if (consume(&file_out, cmdline, index, NULL) == 0) {
		PART_REQUIRE(parse_outfile(parse, cmdline, index));
	}
	return 0;
}

// --------------------- MAIN PARSE FUNCTION ----------------------

/**
 * Parse command input into parse struct
 * 
 * @param cmdline command line input
 *
 * @return parse struct
 */
struct parse* parse_command_input(char* cmdline) {
	// Parse header entered
	PARSE_HEADER()
	
	// Declare index int and allocate parse struct memory
	int index = 0;
	struct parse* parse = parse_new();
	
	// Parse component
	PARSE_REQUIRE(parse_tasks(parse, cmdline, &index))	

	// Exit valid
	PARSE_VALID()
}

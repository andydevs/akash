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

void __debug_parse__printf(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
#ifdef DEBUG_PARSE
	vprintf(fmt, args);
#endif
	va_end(args);
}

const char* file_string = "[a-zA-Z0-9_\\.\\-\\~]+(/[a-zA-Z0-9_\\.\\-]+)?";
regex_t file;

void parse_init() {
	regcomp(&file, file_string, REG_EXTENDED);
}

void parse_deinit() {
	regfree(&file);
}

void print_parse_error(int error, regex_t* regex) {
	size_t errbuf_size = regerror(error, regex, (char*)NULL, (size_t)0);
	char* errbuf = (char*)malloc(errbuf_size*sizeof(char));
	regerror(error, regex, errbuf, errbuf_size);
	printf("ERROR: %s\n", errbuf);
}

int parse_command(struct parse* parse, char* cmdline, int index) {
	// Find match in string at index
	regmatch_t match[1];
	int nindex;
	int error = regexec(&file, &cmdline[index], 1, match, 0);
	if (error) {
		// Handle parse error
		parse->cmd = NULL;
		print_parse_error(error, &file);

		// New index is -1
		nindex = -1;
	}
	else {
		// Add command to parse
		parse->cmd = strndup(&cmdline[match[0].rm_so], match[0].rm_eo - match[0].rm_so);
		__debug_parse__printf("Command: %s\n", parse->cmd);	
		
		// New index is the end of match
		nindex = match[0].rm_eo;
	}

	// Return new index
	return nindex;
}

struct parse* parse_command_input(char* cmdline) {
	__debug_parse__printf("\n==========PARSE==========\n");
	__debug_parse__printf("Entered: %s\n", cmdline);	
	
	// Declare index int and allocate parse struct memory
	int index = 0;
	struct parse* parse = (struct parse*)malloc(sizeof(struct parse));
	
	// Parse command
	index = parse_command(parse, cmdline, index);
	// Exit invalid if index is less than 0
	if (index < 0) {	
		parse->valid = 0;
		__debug_parse__printf("Command line invalid\n");
		__debug_parse__printf("=========================\n\n");
		return parse;
	}
	
	// Exit valid
	parse->valid = 1;
	__debug_parse__printf("Command line valid\n");
	__debug_parse__printf("=========================\n\n");
	return parse;
}

void parse_destroy(struct parse** parse) {
	if ((*parse)->cmd) free((*parse)->cmd);
	free(*parse);
	*parse = NULL;
}

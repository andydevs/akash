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
#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Regex strings
const char* shell_string = "^\\s*!";
const char* shcmd_string = "^\\s*\\w+";
const char* file_string = "^\\s*(\\w|[\\.-~])+(/(\\w|[\\.-])+)*";
const char* arg_string = "^\\s*[^ \t\\|<>&]+";
const char* pipe_string = "^\\s*\\|";
const char* file_in_string = "^\\s*<";
const char* file_out_string = "^\\s*>";
const char* background_string = "&";
const char* whitespace_string = "^\\s*";

// Whitespace token
regex_t whitespace;

// ---------------------------- TOKEN SETUP ------------------------------

/**
 * Initialize parser
 */
void tokenizer_init() {
	regcomp(&shell, shell_string, REG_EXTENDED);
	regcomp(&shcmd, shcmd_string, REG_EXTENDED);
	regcomp(&file, file_string, REG_EXTENDED);
	regcomp(&arg, arg_string, REG_EXTENDED);
	regcomp(&pipe, pipe_string, REG_EXTENDED);
	regcomp(&file_in, file_in_string, REG_EXTENDED);
	regcomp(&file_out, file_out_string, REG_EXTENDED);
	regcomp(&background, background_string, REG_EXTENDED);
	regcomp(&whitespace, whitespace_string, REG_EXTENDED);
}

/**
 * Deinitialize parser
 */
void tokenizer_deinit() {
	regfree(&shell);
	regfree(&shcmd);
	regfree(&file);
	regfree(&arg);
	regfree(&pipe);
	regfree(&file_in);
	regfree(&file_out);
	regfree(&background);
	regfree(&whitespace);
}

// --------------------------- TOKEN HELPERS -----------------------------

/**
 * Print tokenizer error for regex
 *
 * @param error error code
 * @param regex regex causing error
 */
void print_token_error(int error, regex_t* regex) {
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
		print_token_error(error, &whitespace);
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
			// Handle token error
			print_token_error(error, token);
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
	int error = regexec(&arg, &cmdline[*index], 0, NULL, 0);
	if (error != 0 && error != REG_NOMATCH) {
		print_token_error(error, token);
	}
	return error;
}

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
#pragma once
#include <regex.h>

// Token types
regex_t file;
regex_t arg;
regex_t pipe;
regex_t file_in;
regex_t file_out;
regex_t background;

/**
 * Initialize parser
 */
void parse_init();

/**
 * Deinitialize parser
 */
void parse_deinit(); 

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
int consume(regex_t* token, char* cmdline, int* index, char** output); 

/**
 * Lookahead for next token
 *
 * @param token   token regex
 * @param cmdline command input line
 * @param index   starting index of input to check (does not update)
 *
 * @return lookahead result
 */
int lookahead(regex_t* token, char* cmdline, int* index); 

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
#include "parser/parse.h"

/**
 * Initialize execute
 */
void execute_init();

/**
 * Deinitialize execute
 */
void execute_deinit();

/**
 * Execute parsed command represented by parse
 * 
 * @param parse struct representing parsed command
 */
void execute_parsed_command(struct parse* parse); 

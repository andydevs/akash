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

struct arg_node {
	char* text;
	struct arg_node* next;
};

struct task {
	char* cmd;
	struct arg_node* args;
};

struct parse {
	int valid;
	struct task* task;
};

void parse_destroy(struct parse* parse);

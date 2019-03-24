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

void print_head(void) {
	printf("\n");
	printf("||                                             ||\n");
	printf("||           ^    |  /    ^    /-- |           ||\n");
	printf("||          / \\   | /    / \\   |_  |--\\        ||\n");
	printf("||         /---\\  | \\   /---\\    \\ |  |        ||\n");
	printf("||        /     \\ |  \\ /     \\ __/ |  |        ||\n");
	printf("||        Andy Kharbandy's Average Shell       ||\n");
	printf("||                                             ||\n");
	printf("||        Type ctrl+D or 'exit' to exit        ||\n");
	printf("||                                             ||\n");
	printf("\n");
}


/**
 * Program entry point
 *
 * @param argc number of command arguments
 * @param argv command arguments
 * 
 * @return exit status
 */
int main(int argc, const char** argv) {
	print_head();
	return 0;
}

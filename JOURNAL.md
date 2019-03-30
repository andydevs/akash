Andy Kharbandy's Average Shell (AKASh): Journal
===============================================

----------------------------------------------------------------------------------------------

So this is based on a project for my systems programming class, but I wanted to continue with something similar because it was fun (and I couldn't finish the other one, so...).

So was created Andy Kharbandy's Average Shell (or AKASh, and yes I know it's a name. It's only semi-coincidental).

As of writing this journal, I have a banner that prints at the start of the terminal, I have input reading, and I have simple command parsing.

I also finally got a linode account. So I'm working on this in a linode server.

----------------------------------------------------------------------------------------------

So the parser I have set up is made up of functions which parse different parts of the command line.

```c
int parse_part(struct parse* parse, char* cmdline, int index) {
	regex_t part;
	regcomp(...);

	// Find match
	
	regfree(...);

	// Return new index
}
```

Basically, it parses the part and adds it to the parse struct. The final argument is the index from which to start parsing. The function returns the next index after the token. These are chained together by starting the index at 0 and updating the index.

The problem with this setup is that the regex is compiled everytime you run the function, which could be multiple times in a single command. So, we'd have to move these regexes into globalspace within the program.

This would mean we'd have to initialize and deinitialize the regexes in functions, which would be called at the start and end of main respectively.

```c
const char* part_string = "regex for part...";
regex_t part;
.
.
.

void parse_init() {
	regcomp(&part, part_string, REG_EXTENDED);
	.
	.
	.
}

int parse_part(struct parse* parse, char* cmdline, int index) {
	// Find match
	// Return new index
}

void parse_deinit() {
	regfree(&part);
	.
	.
	.
}
```

----------------------------------------------------------------------------------------------

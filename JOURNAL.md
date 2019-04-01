Andy Kharbandy's Average Shell (AKASh): Journal
===============================================

---------------------------------------------------------------------------------

So this is based on a project for my systems programming class, but I wanted to 
continue with something similar because it was fun (and I couldn't finish the 
other one, so...).

So was created Andy Kharbandy's Average Shell (or AKASh, and yes I know it's a 
name. It's only semi-coincidental).

As of writing this journal, I have a banner that prints at the start of the 
terminal, I have input reading, and I have simple command parsing.

I also finally got a linode account. So I'm working on this in a linode server.

---------------------------------------------------------------------------------

So the parser I have set up is made up of functions which parse different parts 
of the command line.

```c
int parse_part(struct parse* parse, char* cmdline, int index) {
	regex_t part;
	regcomp(...);

	// Find match
	
	regfree(...);

	// Return new index
}
```

Basically, it parses the part and adds it to the parse struct. The final argument 
is the index from which to start parsing. The function returns the next index 
after the token. These are chained together by starting the index at 0 and 
updating the index.

The problem with this setup is that the regex is compiled everytime you run the 
function, which could be multiple times in a single command. So, we'd have to 
move these regexes into globalspace within the program.

This would mean we'd have to initialize and deinitialize the regexes in 
functions, which would be called at the start and end of main respectively.

```c
const char* part_string = "regex for part...";
.
.
.

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

void parse_deinit() {
	regfree(&part);
	.
	.
	.
}

int parse_part(struct parse* parse, char* cmdline, int index) {
	// Find match
	// Return new index
}
```

---------------------------------------------------------------------------------

I'm having an issue with the setup I got that trims the string to the new search
start. regexec seems to be using the entire string cmdline despite me manually 
trimming the string with &cmdline[index].

I think it may be order of operations. the & operation is done first and then [],
so we're just getting the value of the address of cmdline at `index`, which makes
no sense. I'll try adding parentheticals (so it looks like &(cmdline[index]))

Nap... still the same.

---------------------------------------------------------------------------------

You know what we gotta do? Substrings....

---------------------------------------------------------------------------------

And guess what, it works!

Now we got another issue... arg is processing whitespace. We need to ignore that.

---------------------------------------------------------------------------------

The \b match seems to be helping with the start but not the end

---------------------------------------------------------------------------------

Looks like I'm gonna have to manually enter every character that's possible in
the arg part

---------------------------------------------------------------------------------

Nvm. Using \S to match all non-spaces seems to work!

---------------------------------------------------------------------------------

I just noticed something: when regexec saves the match indeces, they're saved
relative to the start of the _substring_ rather than the string as a whole, so
you'd have to add index to the match start offset to get the actual starting 
index of the match. That's probably why I was getting mismatched matches for 
arg.

Let's see if this works...

---------------------------------------------------------------------------------

It works! Wow!

I always imagine someone watching me make that error and has been screaming at me
the entire time I was struggling with this.

Story of my life 

---------------------------------------------------------------------------------

So I'm gonna make a modification to how my system works. Instead of returning the
new index, the function sets the index inside it. We pass the pointer of the index
to the function and it is updated internally. What is returned is an int representing
the match result.

---------------------------------------------------------------------------------

Alright! Seems to work.

I'm gonna hold off on reorganizing the parser so I can focus on adding more
features. That could be a job for lex/yacc.

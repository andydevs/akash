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

---------------------------------------------------------------------------------

The next thing I want to do is split cases for just not matching and actually
having an error in the parse statements.

---------------------------------------------------------------------------------

You know what would work here... switch satements.

---------------------------------------------------------------------------------

It works.

Why do I keep focusing on small bullshit? This is why I can't do anything 
substantial.

---------------------------------------------------------------------------------

Parsing multiple arguments works for the most part. Pipes are considered part of
arguments. Have to fix that.

---------------------------------------------------------------------------------

...fixed

---------------------------------------------------------------------------------

Okay... time to work on pipes finally...

First things first, I need to implement the task struct using our current system.
I'll have to move a lot of code into a `parse_task` function that handles task
parsing

What if I abstracted some of the code for each of these parts into...

NO! NUNADAT! Features first!

---------------------------------------------------------------------------------

So the task struct is implemented, for one task.

---------------------------------------------------------------------------------

I'm getting to this point where I don't want to do this anymore. Things are not
really going anywhere and It's more getting in the way than actually contributing.

I can take this on at any pace I like... It's okay.

---------------------------------------------------------------------------------

WOW! It's been forever! But I'm back boi.

Multiple tasks via pipe... that's what I gotta do

I have the infrastructure for adding multiple pipes... Now I just gotta parse 
pipes

Looks like I have part of the pipe parsing implemented already! So I just had to
add a couple more functions. However, I realized that `task_append_arg` was
needlessly complicated. If I prepend each task, like with args, it makes the code
a lot easier. I can reverse it later. So I replaced `task_append_arg` with
`task_prepend_arg`

Other than that, parsing multiple tasks works!. Now I just gotta do file IO.

---------------------------------------------------------------------------------

Since file IO can be done in the `parse_tasks` function, I'll rename that to
`parse_command` or `parse_input` or something, since that's the main parse input
system.

---------------------------------------------------------------------------------

What if I changed the regex tokens to match the immediate next string rather than
anywhere in the string

... surprising... that works for once...

---------------------------------------------------------------------------------

How do I do unit tests???

---------------------------------------------------------------------------------

So the parse.c file was getting to big, so... you guessed it! Split the code!

I created a separate .h and .c file called "tokenizer" to hold all of the token
parsing code. I also created generic functions which would parse separate tokens.
Changing the regexes to match the immediate next string helped with that.

The parse file looks so much cleaner because of it.

By the way wouldn't it make sense for parse.c/h to be called parser.c/h and vice
versa? I'll change that soon.

---------------------------------------------------------------------------------

So parsing is all done now! I'll celebrate by making the banner look fancy!

After that, it's onto execution

---------------------------------------------------------------------------------

Well I'll have to find a way to replace the linked lists with static arrays...

OR I can find a way to create a static array during execution...

---------------------------------------------------------------------------------

There are times when I can't seem to split into separate functions... or rather,
it's gonna take a bit of creativity

---------------------------------------------------------------------------------

It's hard to remember to type my thoughts out into this journal...

Anyways, I was able to split up the args building code into separate functions...
so that's cool.

I also got multiple arguments to work.

My next move is to have the tasks run asynchronously using SIGCHILD to wait on
them.

---------------------------------------------------------------------------------

So it appears I cannot use SIGCHLD (not SIGCHILD, forgot there's no I), since
that screws up the prompt thing... so, I'll just have the system wait afterwards.
This way it will start all processes so we won't have a deadlock-ish situation
if someone has pipes going on.

---------------------------------------------------------------------------------

Meanwhile, I discovered an error in the program. If I type in a blank line, the
program errors... so I'll have to get rid of that...

Fixed... just had to check if the line wasn't empty before proceeding...

---------------------------------------------------------------------------------

OH MY GOD! I have had the dumbest code error that I have ever seen!

So I've been trying to implement pipes for each of the tasks being run. I had
something like this:

```c
int pipe_fd[size-1][2];
for (int i = 0; i < size-1; i++) {
	pipe(pipe_fd[i]);
}
```

I would close the files later, but, point being, I kept getting a segfault error
whenever this calls...

So I just tried one pipe

```c
int fd[2];
pipe(fd);
close(fd[0]);
close(fd[1]);
```

Still the same error...

So I tried it in the main function... Still the same goddamn error!

At this point, I'm frantically trying to google "pipe segmentation fault" and
getting nothing. I was about to give up... until I noticed something.

The term "pipe" in the program was _also_ declared in tokenizer.h (the pipe
token), which was being imported in akash.c, _overwriting_ the pipe function
and causing the segfault when I call it...

Jesus...

Changed "pipe" in tokenizer.h to "piper" (it works, whatever). Error was fixed...

I do this for fun, ladies and gents... My life is a joke.

---------------------------------------------------------------------------------

Hallelujiah pipes work!

I've created a separate file descriptor management system which creates the entire
IO chain separately before handing it to `handle_child`. Should be easy to debug.

---------------------------------------------------------------------------------

File IO was easy...

---------------------------------------------------------------------------------

I created a new debug system. Rather than having blocks, I decided to go for the
node debug approach and create headers before debug statements... multicolored
headers! I also created a header file which creates custom debug statements with
the headers... I might have overengineered this, but it's alright. It looks cool!

---------------------------------------------------------------------------------

Well, now my goal is to work on the build system, i.e. splitting code into separate
libraries.

I first want to see if I can use ld instead of gcc for linking files

---------------------------------------------------------------------------------

So I've done a few things since the last entry. This is gonna be a long entry

The first thing, `ld` doesn't work... I had to use `gcc` for program. However,
`ar rcs` works for libraries!

So in the beginning my project folder looked like this

	akash
	  | akash.c
	  | parser.h
	  | parser.c
	  | parse.h
	  | parse.c
	  | tokenizer.h
	  | tokenizer.c
	  | execute.h
	  | execute.c
	  | taskslist.h
	  | taskslist.c
	  | argslist.h
	  | argslist.c
	  | io.h
	  | io.c
	  | shellcommand.h
	  | shellcommand.c
	  | Makefile

Here was my Makefile

```make
SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.o,%.c)
FLAGS=-g -lreadline $(patsubst %,-D%,$(DEBUG))

akash: $(OBJECTS)
	gcc -o $@ $^ $(FLAGS)

%.o: %.c
	gcc -c -o $@ $^

run: akash
	./akash

debug: akash
	gdb akash

clean:
	rm -f akash
	rm -f *.o
```

I started work by one-by-one moving source code into separate libraries in the
Makefile. I eventually got a Makefile like this.

```make
LIBS=-L. -lparser -lexecute -lshellcommand
FLAGS=-g $(patsubst %,-D%,$(DEBUG))

akash: akash.o libparser.a libexecute.a libshellcommand.a
	gcc -o akash akash.o $(LIBS) $(FLAGS) 

libparser.a: parser.o parse.o tokenizer.o
	ar rcs $@ $^

libexecute.a: execute.o taskslist.o argslist.o io.o
	ar rcs $@ $^

libshellcommand.a: shellcommand.o
	ar rcs $@ $^

%.o: %.c
	gcc -c -o $@ $^ $(FLAGS)

run: akash
	./akash

debug: akash
	gdb akash

clean:
	rm -f akash
	rm -f *.a
	rm -f *.o
```

I then moved each library's source code into each folder with it's own Makefile.
In the main Makefile, I used the command `$(MAKE) [target] -C [subdir]` to call
the Makefile within the directory.

Here's the Makefile:


```make
LIBDIRS=-Lparser -Lexecute -Lshellcommand
LIBS=-lparser -lexecute -lshellcommand
FLAGS=-g $(patsubst %,-D%,$(DEBUG))

.PHONY: run debug clean

akash: akash.o parser/libparser.a execute/libexecute.a shellcommand/libshellcommand.a
	gcc -o akash akash.o $(LIBDIRS) $(LIBS) $(FLAGS) 

parser/libparser.a:
	$(MAKE) libparser.a -C parser

execute/libexecute.a:
	$(MAKE) libexecute.a -C execute

shellcommand/libshellcommand.a:
	$(MAKE) libshellcommand.a -C shellcommand

run: akash
	./akash

debug: akash
	gdb akash

clean:
	rm -f akash
	rm -f *.o
	$(MAKE) clean -C parser
	$(MAKE) clean -C execute
	$(MAKE) clean -C shellcommand 
```

And here's the directory structure

	akash
	  | akash.c
	  | parser
	  ----| parser.h
		  | parser.c
		  | parse.h
		  | parse.c
		  | tokenizer.h
		  | tokenizer.c
		  | Makefile
	  | execute
	  ----| execute.h
		  | execute.c
		  | taskslist.h
		  | taskslist.c
		  | argslist.h
		  | argslist.c
		  | io.h
		  | io.c 
		  | Makefile
	  | shellcommand
	  ----| shellcommand.h
		  | shellcommand.c
		  | Makefile
	  | Makefile

The Makefile in each library folder follows this format:

```make
FLAGS=-g $(patsubst %,-D%,$(DEBUG))

libparser.a: parser.o parse.o tokenizer.o
	ar rcs $@ $^

%.o: %.c
	gcc -c -o $@ $^ $(FLAGS)

clean:
	rm -f *.a *.o
```

Of course `libparser.a` and it's sources will change for each target.

I ran into a problem around this time. The libraries won't compile unless
they don't exist. In short, I had to force the execution of the subdirectories
in order to keep the libraries up to date. You do this by adding a bogus
`.FORCE` target which will always run (since no target ever gets made)

```make
LIBDIRS=-Lparser -Lexecute -Lshellcommand
LIBS=-lparser -lexecute -lshellcommand
FLAGS=-g $(patsubst %,-D%,$(DEBUG))

.PHONY: run debug clean
.FORCE:

akash: akash.o parser/libparser.a execute/libexecute.a shellcommand/libshellcommand.a
	gcc -o akash akash.o $(LIBDIRS) $(LIBS) $(FLAGS) 

parser/libparser.a: .FORCE
	$(MAKE) libparser.a -C parser

execute/libexecute.a: .FORCE
	$(MAKE) libexecute.a -C execute

shellcommand/libshellcommand.a: .FORCE
	$(MAKE) libshellcommand.a -C shellcommand

run: akash
	./akash

debug: akash
	gdb akash

clean:
	rm -f akash
	rm -f *.o
	$(MAKE) clean -C parser
	$(MAKE) clean -C execute
	$(MAKE) clean -C shellcommand 
```

Then I realized that the main akash executable can be in it's own folder as well. This makes the main Makefile responsible only for orchestrating the subdir makes. Here's the new folder structure

	akash
	  | akash
	  ----| akash.c
		  | Makefile
	  | parser
	  ----| parser.h
		  | parser.c
		  | parse.h
		  | parse.c
		  | tokenizer.h
		  | tokenizer.c
		  | Makefile
	  | execute
	  ----| execute.h
		  | execute.c
		  | taskslist.h
		  | taskslist.c
		  | argslist.h
		  | argslist.c
		  | io.h
		  | io.c 
		  | Makefile
	  | shellcommand
	  ----| shellcommand.h
		  | shellcommand.c
		  | Makefile
	  | Makefile

And here's the Makefile

```make
.PHONY: run debug clean
.FORCE:

akash/akash: parser/libparser.a execute/libexecute.a shellcommand/libshellcommand.a
	$(MAKE) akash -C akash	

parser/libparser.a: .FORCE
	$(MAKE) libparser.a -C parser

execute/libexecute.a: .FORCE
	$(MAKE) libexecute.a -C execute

shellcommand/libshellcommand.a: .FORCE
	$(MAKE) libshellcommand.a -C shellcommand

run: akash/akash
	./akash/akash

debug: akash/akash
	gdb akash/akash

clean:
	$(MAKE) clean -C akash
	$(MAKE) clean -C parser
	$(MAKE) clean -C execute
	$(MAKE) clean -C shellcommand 
```

I could add the Makefile code for akash in the subdirectory of akash.

```make
LIBDIRS=-L../parser -L../execute -L../shellcommand
LIBRARIES=-lreadline -lparser -lexecute -lshellcommand
FLAGS=-g $(patsubst %,-D%,$(DEBUG))

.PHONY: clean
.FORCE:

akash: akash.o .FORCE
	gcc -o akash akash.o $(LIBDIRS) $(LIBRARIES) $(FLAGS)

%.o: %.c
	gcc -c -o $@ $^ $(FLAGS)

clean:
	rm -f akash akash.o
```

Finally, I've added a few header files. There's debug.h which handles debugging, and version.h which contains info about the program. As such I have to add the include path to the build (using flag `-I..`).

```make
LIBDIRS=-L../parser -L../execute -L../shellcommand
LIBRARIES=-lreadline -lparser -lexecute -lshellcommand
FLAGS=-I.. -g $(patsubst %,-D%,$(DEBUG))

.PHONY: clean
.FORCE:

akash: akash.o .FORCE
	gcc -o akash akash.o $(LIBDIRS) $(LIBRARIES) $(FLAGS)

%.o: %.c
	gcc -c -o $@ $^ $(FLAGS)

clean:
	rm -f akash akash.o
```

```make
FLAGS=-I.. -g $(patsubst %,-D%,$(DEBUG))

libparser.a: parser.o parse.o tokenizer.o
	ar rcs $@ $^

%.o: %.c
	gcc -c -o $@ $^ $(FLAGS)

clean:
	rm -f *.a *.o
```

So this is what I've done for splitting up the code. 

---------------------------------------------------------------------------------

Oh, and I've also made the beemoviescript program. I decided that, since which is
a gnu-standard program.

	akash
	  | akash
	  ----| akash.c
		  | Makefile
	  | parser
	  ----| parser.h
		  | parser.c
		  | parse.h
		  | parse.c
		  | tokenizer.h
		  | tokenizer.c
		  | Makefile
	  | execute
	  ----| execute.h
		  | execute.c
		  | taskslist.h
		  | taskslist.c
		  | argslist.h
		  | argslist.c
		  | io.h
		  | io.c 
		  | Makefile
	  | shellcommand
	  ----| shellcommand.h
		  | shellcommand.c
		  | Makefile
	  | beemovescript
	  ----| beemoviescript.c
		  | Makefile
	  | Makefile

_Makefile in base_

```make
.PHONY: run debug clean
.FORCE: 

all: akash/akash beemoviescript/beemoviescript

akash/akash: parser/libparser.a execute/libexecute.a shellcommand/libshellcommand.a .FORCE
	$(MAKE) akash -C akash

beemoviescript/beemoviescript: .FORCE
	$(MAKE) beemoviescript -C beemoviescript

shellcommand/libshellcommand.a: .FORCE
	$(MAKE) libshellcommand.a -C shellcommand

execute/libexecute.a: parser/libparser.a .FORCE
	$(MAKE) libexecute.a -C execute

parser/libparser.a: .FORCE
	$(MAKE) libparser.a -C parser

run: akash/akash
	./akash/akash

debug: akash/akash
	gdb akash/akash

clean:
	$(MAKE) clean -C akash
	$(MAKE) clean -C shellcommand
	$(MAKE) clean -C execute
	$(MAKE) clean -C parser
	$(MAKE) clean -C beemoviescript
```

_Makefile in beemoviescript_

```make
FLAGS=-I.. -g $(patsubst %,-D%,$(DEBUG))

.PHONY: clean run

beemoviescript: beemoviescript.o
	gcc -o $@ $^

%.o: %.c
	gcc -c -o $@ $^ $(FLAGS)

run: beemoviescript
	./beemoviescript

clean:
	rm -f beemoviescript *.o
```

FLAGS=-g $(patsubst %,-D%,$(DEBUG))
LIBRARIES=-lreadline -lparser -lexecute -lshellcommand

.PHONY: run debug clean

akash: akash.o libparser.a libexecute.a libshellcommand.a
	gcc -o $@ akash.o -L. $(LIBRARIES) 

libshellcommand.a: shellcommand.o
	ar rcs $@ $^

libexecute.a: execute.o argslist.o taskslist.o io.o
	ar rcs $@ $^

libparser.a: parser.o tokenizer.o parse.o
	ar rcs $@ $^

%.o: %.c
	gcc -c -o $@ $< $(FLAGS)

run: akash
	./akash

debug: akash
	gdb akash

clean:
	rm -f akash *.o *.a

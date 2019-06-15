.PHONY: run debug clean

akash: akash.o parser.o tokenizer.o parse.o execute.o argslist.o taskslist.o io.o shellcommand.o
	gcc -o $@ $^ -lreadline -g $(patsubst %,-D%,$(DEBUG))

%.o: %.c
	gcc -c -o $@ $< $(FLAGS)

run: akash
	./akash

debug: akash
	gdb akash

clean:
	rm -f akash akash.o parser.o tokenizer.o parse.o execute.o argslist.o taskslist.o io.o shellcommand.o

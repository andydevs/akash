FLAGS=-I.. -g $(patsubst %,-D%,$(DEBUG))

.PHONY: clean

libexecute.a: execute.o taskslist.o argslist.o io.o
	ar rcs $@ $^

%.o: %.c
	gcc -c -o $@ $^ $(FLAGS)

clean:
	rm -f *.a *.o

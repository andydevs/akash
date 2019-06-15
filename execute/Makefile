INCLUDE=-I..

libexecute.a: execute.o taskslist.o argslist.o io.o
	ar rcs $@ $^

%.o: %.c
	gcc -c -o $@ $^ $(INCLUDE)

clean:
	rm -f *.a *.o

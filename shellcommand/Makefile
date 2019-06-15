INCLUDE=-I..

libshellcommand.a: shellcommand.o
	ar rcs $@ $^

%.o: %.c
	gcc -c -o $@ $^ $(INCLUDE)

clean:
	rm -f *.a *.o

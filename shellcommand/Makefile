FLAGS=-I.. -g $(patsubst %,-D%,$(DEBUG))

.PHONY: clean

libshellcommand.a: shellcommand.o
	ar rcs $@ $^

%.o: %.c
	gcc -c -o $@ $^ $(FLAGS)

clean:
	rm -f *.a *.o

FLAGS=-I.. -g $(patsubst %,-D%,$(DEBUG))

.PHONY: clean

libparser.a: parser.o parse.o tokenizer.o
	ar rcs $@ $^

%.o: %.c
	gcc -c -o $@ $^ $(FLAGS)

clean:
	rm -f *.a *.o

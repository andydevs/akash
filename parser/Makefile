INCLUDE=-I..

libparser.a: parser.o parse.o tokenizer.o
	ar rcs $@ $^

%.o: %.c
	gcc -c -o $@ $^ $(INCLUDE)

clean:
	rm -f *.a *.o

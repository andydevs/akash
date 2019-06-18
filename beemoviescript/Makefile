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

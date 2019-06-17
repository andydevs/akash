FLAGS=-I.. -g $(patsubst %,-D%,$(DEBUG))
LIBDIRS=-L../parser -L../execute -L../shellcommand
LIBRARIES=-lreadline -lparser -lexecute -lshellcommand

.PHONY: clean

akash: akash.o
	gcc -o akash akash.o $(LIBDIRS) $(LIBRARIES) $(FLAGS)

%.o: %.c
	gcc -c -o $@ $^ $(FLAGS)

clean:
	rm -f akash akash.o

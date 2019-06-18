LIBDIRS=-L../parser -L../execute -L../shellcommand
LIBRARIES=-lreadline -lparser -lexecute -lshellcommand
FLAGS=-I.. -g $(patsubst %,-D%,$(DEBUG))

.PHONY: clean
.FORCE:

akash: akash.o .FORCE
	gcc -o akash akash.o $(LIBDIRS) $(LIBRARIES) $(FLAGS)

%.o: %.c
	gcc -c -o $@ $^ $(FLAGS)

clean:
	rm -f akash akash.o

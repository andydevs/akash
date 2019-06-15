FLAGS=-I. -g $(patsubst %,-D%,$(DEBUG))
LIBDIRS=-Lparser -Lexecute -Lshellcommand
LIBRARIES=-lreadline -lparser -lexecute -lshellcommand

.PHONY: run debug clean

akash: akash.o parser/libparser.a execute/libexecute.a shellcommand/libshellcommand.a
	gcc -o $@ akash.o $(LIBDIRS) $(LIBRARIES) 

shellcommand/libshellcommand.a:
	$(MAKE) libshellcommand.a -C shellcommand

execute/libexecute.a:
	$(MAKE) libexecute.a -C execute

parser/libparser.a:
	$(MAKE) libparser.a -C parser

%.o: %.c
	gcc -c -o $@ $< $(FLAGS)

run: akash
	./akash

debug: akash
	gdb akash

clean:
	rm -f akash *.o
	$(MAKE) clean -C shellcommand
	$(MAKE) clean -C execute
	$(MAKE) clean -C parser

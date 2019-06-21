.PHONY: run debug clean install
.FORCE: 

all: akash/akash beemoviescript/beemoviescript

akash/akash: parser/libparser.a execute/libexecute.a shellcommand/libshellcommand.a .FORCE
	$(MAKE) akash -C akash

beemoviescript/beemoviescript: .FORCE
	$(MAKE) beemoviescript -C beemoviescript

shellcommand/libshellcommand.a: .FORCE
	$(MAKE) libshellcommand.a -C shellcommand

execute/libexecute.a: parser/libparser.a .FORCE
	$(MAKE) libexecute.a -C execute

parser/libparser.a: .FORCE
	$(MAKE) libparser.a -C parser

run: akash/akash
	./akash/akash

debug: akash/akash
	gdb akash/akash

clean:
	$(MAKE) clean -C akash
	$(MAKE) clean -C shellcommand
	$(MAKE) clean -C execute
	$(MAKE) clean -C parser
	$(MAKE) clean -C beemoviescript

install: all
	cp akash/akash /usr/bin
	cp beemoviescript/beemoviescript /usr/bin

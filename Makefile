.PHONY: run debug clean

akash/akash: parser/libparser.a execute/libexecute.a shellcommand/libshellcommand.a
	$(MAKE) akash -C akash

shellcommand/libshellcommand.a:
	$(MAKE) libshellcommand.a -C shellcommand

execute/libexecute.a:
	$(MAKE) libexecute.a -C execute

parser/libparser.a:
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

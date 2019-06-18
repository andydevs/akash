.PHONY: run debug clean
.FORCE: 

akash/akash: parser/libparser.a execute/libexecute.a shellcommand/libshellcommand.a .FORCE
	$(MAKE) akash -C akash

shellcommand/libshellcommand.a: .FORCE
	$(MAKE) libshellcommand.a -C shellcommand

execute/libexecute.a: .FORCE
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

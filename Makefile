all: test

copy_opts:
	cp myopts2struct.h.example myopts2struct.h
	
example: example.c opts2struct.c opts2struct.h myopts2struct.h
	cc -Wall -std=c11 -o $@ *.c
	@echo "make test to see different ways to invoke $@"
clean:
	rm -f *.o *~ example myopts2struct.h

test: copy_opts example
	@echo "*****************************************"
	@echo --- Form "[-*]key=value"  ---
	@echo can use 0 or more hyphens on key=value
	@echo and they can be ints, floats, or strings:
	@echo "*****************************************"
	./example --bunnies=8 stars=3.5 -dogs=cockapoo
	@echo "*****************************************"
	@echo the types are determined dynamically:
	@echo --- non-keys are ignored ---
	@echo "*****************************************"
	./example --thiswillbeignored --dogs=retriever thistoo
	@echo "*****************************************"
	@echo --- Flags ---
	@echo you can use key-only flags:
	@echo "*****************************************"
	./example -bunnies stars=300 -dogs=cockapoo
	@echo "*****************************************"
	@echo use the word "no" to negate the flag:
	@echo "*****************************************"
	./example -nobunnies stars=300 -dogs=cockapoo
	@echo "*****************************************"
	@echo or explicitly use "true" or "false" in key=value:
	@echo "*****************************************"
	./example --bunnies=brown stars=300 dogs="false"
	@echo "*****************************************"
	@echo --- Defaults ---
	@echo this one fills in all the defaults:
	@echo "*****************************************"
	./example
	@echo "*****************************************"
	@echo this one fills in just the one for "stars":
	@echo "*****************************************"
	./example --bunnies=8 -dogs=cockapoo
	@echo "*****************************************"
	@echo --- Form "-key value" shortnames ---
	@echo requires a single hyphen for the key:
	@echo "*****************************************"
	./example -b 8 -s 3.5 -d cockapoo
	@echo "*****************************************"
	./example -b brown -s 300 -d false
	@echo "*****************************************"
	./example -nob -s 300 -d cockapoo
	@echo "*****************************************"
	@echo "remove myopts2struct.h so you can use yours"
	rm -f myopts2struct.h
	@echo "*****************************************"

example: example.c opts2struct.c opts2struct.h myopts2struct.h
	cc -Wall -std=c11 -o example example.c opts2struct.c
	@echo "to test, try something like ./example --bunnies=8 stars=3.5 -dogs=cockapoo"
clean:
	rm -f *.o *~ example

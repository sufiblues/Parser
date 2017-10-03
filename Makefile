# Trivial makefile for the calculator scanner/parser.
# Depends on default (built-in) rules for C compilation.
# Note that rule for goal (parse) must be the first one in this file.


testfour: parse.o
	
	./parse echo 1 input4.txt
	
testthree: parse.o
	
	./parse echo 1 input3.txt


test: parse.o
	
	./parse echo 1 input1.txt


parse.o:
	g++ parse.cpp -o parse




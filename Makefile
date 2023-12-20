#!/usr/local/bin/wsbasic
CXX         = g++ 
FLAGS       = -O2 -Wall
INSTALL_DIR = /usr/local/bin/

all: wsbasic compiler_poc

var.o: var.cpp var.h
	$(CXX) $(FLAGS) -c var.cpp

lexer.o : lexer.cpp lexer.h
	$(CXX) $(FLAGS) -c lexer.cpp

parser.o: parser.cpp parser.h
	$(CXX) $(FLAGS) -c parser.cpp

treenode.o: treenode.cpp treenode.h
	$(CXX) $(FLAGS) -c treenode.cpp
	
executer.o: executer.cpp executer.h
	$(CXX) $(FLAGS) -c executer.cpp

compiler.o: compiler.cpp compiler.h
	$(CXX) $(FLAGS) -c compiler.cpp

wsbasic: main.cpp var.o lexer.o parser.o treenode.o executer.o compiler.o
	$(CXX) $(FLAGS) main.cpp -o wsbasic lexer.o parser.o var.o treenode.o executer.o compiler.o

treetest: treenode.o treetest.cpp
	$(CXX) $(FLAGS) -o treetest treetest.cpp treenode.o

compiler_poc: compiler_poc.cpp
	$(CXX) $(FLAGS) -o compiler_poc compiler_poc.cpp
	@echo "make testcompile   -> proof of concept on how we can compile real executables"

testcompile:
	@./compiler_poc
	@./out
	echo $?

install:
	cp wsbasic $(INSTALL_DIR)

clean:
	@rm -vf *~ *.o a.out core wsbasic treetest
	@rm -vf scripts/*~
	@rm -vf out out.asm out.o compiler_poc


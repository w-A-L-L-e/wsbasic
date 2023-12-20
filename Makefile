#!/usr/local/bin/wsbasic
CXX         = g++ 
FLAGS       = -O2 -Wall
INSTALL_DIR = /usr/local/bin/

all: wsbasic compiler_poc

var.o: src/var.cpp src/var.h
	$(CXX) $(FLAGS) -c src/var.cpp

lexer.o : src/lexer.cpp src/lexer.h
	$(CXX) $(FLAGS) -c src/lexer.cpp

parser.o: src/parser.cpp src/parser.h
	$(CXX) $(FLAGS) -c src/parser.cpp

treenode.o: src/treenode.cpp src/treenode.h
	$(CXX) $(FLAGS) -c src/treenode.cpp
	
executer.o: src/executer.cpp src/executer.h
	$(CXX) $(FLAGS) -c src/executer.cpp

compiler.o: src/compiler.cpp src/compiler.h
	$(CXX) $(FLAGS) -c src/compiler.cpp

wsbasic: src/main.cpp var.o lexer.o parser.o treenode.o executer.o compiler.o
	$(CXX) $(FLAGS) src/main.cpp -o wsbasic lexer.o parser.o var.o treenode.o executer.o compiler.o

treetest: src/treenode.o src/treetest.cpp
	$(CXX) $(FLAGS) -o treetest src/treetest.cpp treenode.o

compiler_poc: src/compiler_poc.cpp
	$(CXX) $(FLAGS) -o compiler_poc src/compiler_poc.cpp
	@echo "make testcompile   -> proof of concept on how we can compile real executables"

testcompile:
	@./compiler_poc
	@./out
	echo $?

install:
	cp wsbasic $(INSTALL_DIR)

clean:
	@rm -vf *~ *.o a.out core wsbasic treetest compiler_poc
	@rm -vf scripts/*~
	@rm -vf out out.asm out.o


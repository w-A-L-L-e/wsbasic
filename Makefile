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

asm_compiler.o: src/asm_compiler.cpp src/asm_compiler.h
	$(CXX) $(FLAGS) -c src/asm_compiler.cpp

c_compiler.o: src/c_compiler.cpp src/c_compiler.h
	$(CXX) $(FLAGS) -c src/c_compiler.cpp


wsbasic: src/main.cpp var.o lexer.o parser.o treenode.o executer.o asm_compiler.o c_compiler.o
	$(CXX) $(FLAGS) src/main.cpp -o wsbasic lexer.o parser.o var.o treenode.o executer.o asm_compiler.o c_compiler.o

treetest: src/treenode.o src/treetest.cpp
	$(CXX) $(FLAGS) -o treetest src/treetest.cpp treenode.o

compiler_poc: src/compiler_poc.cpp
	$(CXX) $(FLAGS) -o compiler_poc src/compiler_poc.cpp
	@echo "make testcompile   -> proof of concept on how we can compile real executables"

testcompile:
	@./compiler_poc
	@./out
	echo $?

debug: wsbasic
	$(CXX) -g src/main.cpp -o wsbasic_debug lexer.o parser.o var.o treenode.o executer.o asm_compiler.o c_compiler.o
	lldb ./wsbasic_debug hello.bas

install:
	cp wsbasic $(INSTALL_DIR)

clean:
	@rm -vf *~ *.o a.out core wsbasic treetest compiler_poc
	@rm -vf scripts/*~
	@rm -vf output output.asm output.cpp output.o


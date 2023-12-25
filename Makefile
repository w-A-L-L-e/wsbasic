#!/usr/local/bin/wsbasic
# FLAGS       = -O2 -Wall -g # DEBUG FLAGS
FLAGS       = -O2 -Wall
CXX         = g++ 
INSTALL_DIR = /usr/local/bin/

OBJECTS = \
	src/main.o \
	src/var.o \
	src/lexer.o \
	src/parser.o \
	src/treenode.o \
	src/executer.o \
	src/asm_compiler.o \
	src/c_compiler.o \
	src/cpp_compiler.o \
	src/string_utils.o

all: wsbasic

%.o: %.cpp
	$(CXX) $(FLAGS) -c $< -o $@

%.o: %.cpp %.h
	$(CXX) $(FLAGS) -c $< -o $@

wsbasic: $(OBJECTS)
	$(CXX) $(FLAGS) -o $@ $(OBJECTS) 

treetest: src/treenode.o src/treetest.cpp
	$(CXX) $(FLAGS) -o treetest src/treetest.cpp treenode.o

compiler_poc: src/compiler_poc.cpp
	$(CXX) $(FLAGS) -o compiler_poc src/compiler_poc.cpp
	echo "make testcompile   -> proof of concept on how we can compile real executables"

testcompile:
	./compiler_poc
	./out
	echo $?

debug: wsbasic
	@echo "MacOS DEBUGGER HOWTO (make sure makefile uses the debug flags see top line in make):"
	@echo ""
	@echo "1. Just type 'b main': set breakpoint on main"
	@echo "2. type 'run' to execute up to main"
	@echo "3. type 'gui' to show current position in your sources nicer."
	@echo "4. use 's' for step and 'n' for next ..."
	@echo ""
	lldb ./wsbasic scripts/compiler_test.b

install:
	cp wsbasic $(INSTALL_DIR)

clean:
	@rm -vf *~ src/*.o a.out core wsbasic treetest compiler_poc
	@rm -vf scripts/*~
	@rm -vf output output.asm output.cpp output.o output.h


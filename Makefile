#!/usr/local/bin/wsbasic
CXX         = g++ 
FLAGS       = -O2 -Wall
INSTALL_DIR = /usr/local/bin/

all: wsbasic

number.o: number.cpp number.h
	$(CXX) $(FLAGS) -c number.cpp

lexer.o : lexer.cpp lexer.h
	$(CXX) $(FLAGS) -c lexer.cpp

parser.o: parser.cpp parser.h
	$(CXX) $(FLAGS) -c parser.cpp

executer.o: executer.cpp executer.h
	$(CXX) $(FLAGS) -c executer.cpp

treenode.o: treenode.cpp treenode.h
	$(CXX) $(FLAGS) -c treenode.cpp
	
wsbasic: main.cpp number.o lexer.o parser.o treenode.o executer.o
	$(CXX) $(FLAGS) main.cpp -o wsbasic lexer.o parser.o treenode.o executer.o number.o

treetest: treenode.o treetest.cpp
	$(CXX) $(FLAGS) -o treetest treetest.cpp treenode.o

install:
	cp wsbasic $(INSTALL_DIR)

clean:
	@rm -vf *~ *.o a.out core wsbasic treetest
	@rm -vf scripts/*~


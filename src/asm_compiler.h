/*============================================================================
author        : Walter Schreppers
filename      : asm_compiler.h
description   : Compile a parsetree into assembler for nasm
bugreport(log): /
=============================================================================*/

#ifndef ASM_COMPILER_H
#define ASM_COMPILER_H

#include "treenode.h"
#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <fstream>

#include <stdio.h>


typedef std::map<std::string,Var>    symtable;
typedef std::map<std::string,TreeNode*> functable;

typedef std::stack<Var> runstack;

//using namespace std;

class AsmCompiler{
  
  public:
  
    //constructor
    //===========
    AsmCompiler(TreeNode*);
    
    //public members
    //==============
    void generate(const std::string& outfile);
    void link(const std::string& asmfile);
  
  
  private:

    //private members
    //===============
    void compile( TreeNode* ); // work in progress... 

    //private locals
    //==============
    TreeNode* tree;
    std::stack<symtable> symbolTables;
    functable       functionTable;  //keep track of functionNode's
    runstack        runStack;       //stores parameters and return value of functions
    
    bool bReturn;  //used for return statements
    bool bBreak;   //used for break statement
};


#endif


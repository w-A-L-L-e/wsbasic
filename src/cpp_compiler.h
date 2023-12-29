/*============================================================================
author        : Walter Schreppers
filename      : ccp_compiler.h
description   : Compile a parsetree into c++ to be compiled to binary at link step
                just call .generate to get cpp output and call link to compile and
                link actual binary
bugreport(log): /
=============================================================================*/

#ifndef CPP_COMPILER_H
#define CPP_COMPILER_H

#include "treenode.h"
#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <fstream>
#include <stdio.h>
#include "string_utils.h"

typedef std::map<std::string,Var>       symtable;
typedef std::map<std::string,TreeNode*> functable;

typedef std::stack<Var> runstack;

//using namespace std;

class CppCompiler{
  
  public:
  
    //constructor
    //===========
    CppCompiler(TreeNode*);
    
    //public members
    //==============
    void generate(const std::string& outfile);
    void link(const std::string& cppfile);
  
  
  private:

    //private members
    //===============
    void compile(TreeNode*, std::ofstream&);  

    void compConstantString ( TreeNode*, std::ofstream& );
    void compBlock      ( TreeNode*, std::ofstream& );
    void compFor        ( TreeNode*, std::ofstream& );
    void compForEach    ( TreeNode*, std::ofstream& );
    void compWhile      ( TreeNode*, std::ofstream& );
    void compIf         ( TreeNode*, std::ofstream& );
    void compPrint      ( TreeNode*, std::ofstream& );
    void compInput      ( TreeNode*, std::ofstream& );
    void compAssign     ( TreeNode*, std::ofstream& );
    void compId         ( TreeNode*, std::ofstream& );
    void compConstant   ( TreeNode*, std::ofstream& );
    
    void compFunction   ( TreeNode*, std::ofstream&, bool returnFunction = false );
    void compRetFunction( TreeNode*, std::ofstream& );
    void compBreak      ( TreeNode*, std::ofstream& );
    void compReturn     ( TreeNode*, std::ofstream& );
    void compExit       ( TreeNode*, std::ofstream& );

    // Var getVal ( TreeNode* ); // to be deprecated
    void compBinaryOperator( TreeNode*, const std::string&, std::ofstream& );
    void compAdd   ( TreeNode*, std::ofstream& );
    void compMul   ( TreeNode*, std::ofstream& );
    void compDiv   ( TreeNode*, std::ofstream& );
    void compSub   ( TreeNode*, std::ofstream& );
    void compMod   ( TreeNode*, std::ofstream& );
    void compMinus ( TreeNode*, std::ofstream& );

    void compGE   ( TreeNode*, std::ofstream& );
    void compGT   ( TreeNode*, std::ofstream& );
    void compLE   ( TreeNode*, std::ofstream& );
    void compLT   ( TreeNode*, std::ofstream& );
    void compNE   ( TreeNode*, std::ofstream& );
    void compEQ   ( TreeNode*, std::ofstream& );
    void compAnd  ( TreeNode*, std::ofstream& );
    void compOr   ( TreeNode*, std::ofstream& );
    void compNot  ( TreeNode*, std::ofstream& );

    void compRun   ( TreeNode*, std::ofstream& );
    void compWrite ( TreeNode*, std::ofstream& );
    void compSubstr( TreeNode*, std::ofstream& );
    
    //private locals
    //==============
    TreeNode* tree;
    std::stack<symtable> symbolTables;
    functable       functionTable;  //keep track of functionNode's
    runstack        runStack;       //stores parameters and return value of functions
    
    bool bReturn;  //used for return statements
    bool bBreak;   //used for break statement
    
    std::ofstream out;
    std::ofstream hdr; // header output.h containing function implementations
};
#endif

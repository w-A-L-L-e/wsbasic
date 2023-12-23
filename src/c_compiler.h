/*============================================================================
author        : Walter Schreppers
filename      : compiler.h
description   : Compile a parsetree into assembler for nasm
bugreport(log): /
=============================================================================*/

#ifndef C_COMPILER_H
#define C_COMPILER_H

#include "treenode.h"
#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <fstream>

#include <stdio.h>


typedef map<string,Var>       symtable;
typedef map<string,TreeNode*> functable;

typedef stack<Var> runstack;

//using namespace std;

class CCompiler{
  
  public:
  
    //constructor
    //===========
    CCompiler(TreeNode*);
    
    //public members
    //==============
    void generate(const string& outfile);
    void link(const string& cppfile);
  
  
  private:

    //private members
    //===============
    void compile(TreeNode*, ofstream&);  

    void compConstantString ( TreeNode*, ofstream& );
    void compBlock      ( TreeNode*, ofstream& );
    void compFor        ( TreeNode*, ofstream& );
    void compForEach    ( TreeNode*, ofstream& );
    void compWhile      ( TreeNode*, ofstream& );
    void compIf         ( TreeNode*, ofstream& );
    void compPrint      ( TreeNode*, ofstream& );
    void compInput      ( TreeNode*, ofstream& );
    void compAssign     ( TreeNode*, ofstream& );
    void compExpression ( TreeNode*, ofstream& );
    void compId         ( TreeNode*, ofstream& );
    void compConstant   ( TreeNode*, ofstream& );
    
    void compFunction   ( TreeNode*, ofstream& );
    void compRetFunction( TreeNode*, ofstream& );
    void compBreak      ( TreeNode*, ofstream& );
    void compReturn     ( TreeNode*, ofstream& );
    void compExit       ( TreeNode*, ofstream& );

    // Var getVal ( TreeNode* ); // to be deprecated
    void compBinaryOperator( TreeNode*, const string&, ofstream& );
    void compAdd   ( TreeNode*, ofstream& );
    void compMul   ( TreeNode*, ofstream& );
    void compDiv   ( TreeNode*, ofstream& );
    void compSub   ( TreeNode*, ofstream& );
    void compMod   ( TreeNode*, ofstream& );
    void compMinus ( TreeNode*, ofstream& );

    void compGE   ( TreeNode*, ofstream& );
    void compGT   ( TreeNode*, ofstream& );
    void compLE   ( TreeNode*, ofstream& );
    void compLT   ( TreeNode*, ofstream& );
    void compNE   ( TreeNode*, ofstream& );
    void compEQ   ( TreeNode*, ofstream& );
    void compAnd  ( TreeNode*, ofstream& );
    void compOr   ( TreeNode*, ofstream& );
    void compNot  ( TreeNode*, ofstream& );

    void compRun   ( TreeNode* );
    void compWrite ( TreeNode* );
    void compSubstr( TreeNode* );
    
    string runCommand( const string& );
        
    //private locals
    //==============
    TreeNode* tree;
    stack<symtable> symbolTables;
    functable       functionTable;  //keep track of functionNode's
    runstack        runStack;       //stores parameters and return value of functions
    
    bool bReturn;  //used for return statements
    bool bBreak;   //used for break statement
    
    ofstream out;
    ofstream hdr; // header output.h containing function implementations
};


#endif


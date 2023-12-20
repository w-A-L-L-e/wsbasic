/*============================================================================
author        : Walter Schreppers
filename      : compiler.h
description   : Compile a parsetree into asm
bugreport(log): /
=============================================================================*/

#ifndef EXECUTER_H
#define EXECUTER_H

#include "treenode.h"
#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <fstream>

#include <stdio.h>


typedef map<string,Var>    symtable;
typedef map<string,TreeNode*> functable;

typedef stack<Var> runstack;

//using namespace std;

class Compiler{
  
  public:
  
    //constructor
    //===========
    Compiler(TreeNode*);
    
    //public members
    //==============
    void run();  
  
  
  private:

    //private members
    //===============
    void compile( TreeNode* );  

    void compBlock      ( TreeNode* );
    void compFor        ( TreeNode* );
    void compForEach    ( TreeNode* );
    void compWhile      ( TreeNode* );
    void compIf         ( TreeNode* );
    void compPrint      ( TreeNode* );
    void compInput      ( TreeNode* );
    void compAssign     ( TreeNode* );
    void compExpression ( TreeNode* );
    void compId         ( TreeNode* );
    void compConstant   ( TreeNode* );
    
    void compFunction   ( TreeNode* );
    void compRetFunction( TreeNode* );
    void compReturn     ( TreeNode* );
    void compBreak      ( TreeNode* );


    //gfx stuff, most likely to be refactored or deprecated soon
    void compGraphics(TreeNode* );
    void mayclearScr(TreeNode* );
    void drawPixel(TreeNode* );
    void createScreenBuffer(TreeNode* );
    void drawPixelBuffer(TreeNode* );
    void swapScreen(TreeNode* );
    void drawRectangle(TreeNode* );


 
    Var getVal ( TreeNode* );
    
    void compAdd  ( TreeNode* );
    void compMul  ( TreeNode* );
    void compDiv  ( TreeNode* );
    void compSub  ( TreeNode* );
    void compMod  ( TreeNode* );
    void compNot  ( TreeNode* );

    void compGE   ( TreeNode* );
    void compGT   ( TreeNode* );
    void compLE   ( TreeNode* );
    void compLT   ( TreeNode* );
    void compNE   ( TreeNode* );
    void compEQ   ( TreeNode* );
    
    void compAnd   ( TreeNode* );
    void compOr    ( TreeNode* );
    void compMinus ( TreeNode* );

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
};


#endif


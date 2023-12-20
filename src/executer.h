/*============================================================================
author        : Walter Schreppers
filename      : executer.h
description   : Execute a parse tree.
bugreport(log):/
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

class Executer{
  
  public:
  
    //constructor
    //===========
    Executer(TreeNode*);
    
    //public members
    //==============
    void run();  
  
  
  private:

    //private members
    //===============
    void execute( TreeNode* );  

    void execBlock      ( TreeNode* );
    void execFor        ( TreeNode* );
    void execForEach    ( TreeNode* );
    void execWhile      ( TreeNode* );
    void execIf         ( TreeNode* );
    void execPrint      ( TreeNode* );
    void execInput      ( TreeNode* );
    void execAssign     ( TreeNode* );
    void execExpression ( TreeNode* );
    void execId         ( TreeNode* );
    void execConstant   ( TreeNode* );
    
    void execFunction   ( TreeNode* );
    void execRetFunction( TreeNode* );
    void execReturn     ( TreeNode* );
    void execBreak      ( TreeNode* );


    //gfx stuff, most likely to be refactored or deprecated soon
    void execGraphics(TreeNode* );
    void mayclearScr(TreeNode* );
    void drawPixel(TreeNode* );
    void createScreenBuffer(TreeNode* );
    void drawPixelBuffer(TreeNode* );
    void swapScreen(TreeNode* );
    void drawRectangle(TreeNode* );


 
    Var getVal ( TreeNode* );
    
    void execAdd  ( TreeNode* );
    void execMul  ( TreeNode* );
    void execDiv  ( TreeNode* );
    void execSub  ( TreeNode* );
    void execMod  ( TreeNode* );
    void execNot  ( TreeNode* );

    void execGE   ( TreeNode* );
    void execGT   ( TreeNode* );
    void execLE   ( TreeNode* );
    void execLT   ( TreeNode* );
    void execNE   ( TreeNode* );
    void execEQ   ( TreeNode* );
    
    void execAnd   ( TreeNode* );
    void execOr    ( TreeNode* );
    void execMinus ( TreeNode* );

    void execRun   ( TreeNode* );
    void execWrite ( TreeNode* );
    void execSubstr( TreeNode* );
    
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


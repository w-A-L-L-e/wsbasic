/*=============================================================================
author        : Walter Schreppers
filename      : main.cpp
description   : Open a file, parse it into a tree and then execute it
=============================================================================*/

#include "parser.h"
#include "executer.h"
#include "c_compiler.h"
#include "asm_compiler.h"

int main(int argc, char** arg){
  if(argc!=2){
    cout<<"Usage: "<<arg[0]<<" FILE "<<endl;
    return 1;
  }
  
  ifstream in(arg[1]);
  if( !in.is_open () ){
    cerr<<"could not open file: "<<arg[1]<<endl;
    return 1;
  }
  
  Parser parser(in);
  
  if( parser.parse() ){
    TreeNode* root=parser.getTree();
    //root->showTree(root); //show parsetree

    // TODO: use commandline args to determine execution (interpreter mode)
    // or asm compiler or c compiler to create executables
    Executer exe(root); //execute this tree
    exe.run();

    // create an nasm compiler instance
    AsmCompiler acomp(root);

    // create a c compiler instance
    CCompiler ccomp(root);
  }
  else{
    cerr<<"Parsing failed!"<<endl;
  }

  return 0;
}


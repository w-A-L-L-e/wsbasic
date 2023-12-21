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
  int argpos = 1;
  if(argc < 2){
    cout <<"Usage: "<<arg[0]<<" [-ast, -cpp, -asm] <WSBASIC SCRIPT> "<<endl;
    cout <<"   -ast : show parse result in absact syntax tree for debugging" << endl;
    cout <<"   -cpp : convert to cpp and then compile + link" << endl;
    cout <<"   -asm : convert to asm for nasm and then compile + link" << endl;
    cout << endl;

    return 1;
  }
 
  bool show_tree = false;
  bool asm_compile = false;
  bool c_compile = false;

  if (string(arg[argpos]) == "-ast"){
    show_tree = true;
    argpos++;
  }

  if (string(arg[argpos]) == "-cpp"){
    c_compile = true;
    argpos++;
  }

  if (string(arg[argpos]) == "-asm") {
    asm_compile = true; 
    argpos++;
  }


  ifstream in(arg[argpos]);
  if( !in.is_open () ){
    cerr<<"could not open file: "<<arg[argpos]<<endl;
    return 1;
  }

  Parser parser(in);
  
  if( parser.parse() ){
    TreeNode* root=parser.getTree();

    if (show_tree) {
      cout << "============================( AST ) ===========================" << endl;
      root->showTree(root);
      cout << "---------------------------------------------------------------" << endl;
    }

    if(asm_compile){
      // create an nasm compiler instance
      cout << "generating asm code and linking..." << endl;
      AsmCompiler compiler(root);
      compiler.generate("output.asm");
      compiler.link("output.asm");
    }
    else if(c_compile){
      cout << "generating c code and linking..." << endl;
      CCompiler compiler(root);
      compiler.generate("output.cpp");
      compiler.link("output.cpp");
    }
    else {
      // directly execute the AST
      Executer exe(root);
      exe.run();
    }
  }
  else{
    cerr<<"Parsing failed!"<<endl;
  }

  return 0;
}


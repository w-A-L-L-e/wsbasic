/*=============================================================================
author        : Walter Schreppers
filename      : main.cpp
description   : Open a file, parse it into a tree and then execute it
=============================================================================*/

#include "parser.h"
#include "executer.h"
#include "cpp_compiler.h"
#include "c_compiler.h"
#include "asm_compiler.h"


int main(int argc, char** arg){
  int argpos = 1;
  if(argc < 2){
    cout <<"usage: "<<arg[0]<<" [-ast, -cpp, -asm] <script> "<<endl;
    cout <<"   -ast : show parse result in absact syntax tree for debugging" << endl;
    cout <<"   -cpp : convert to C++ and then compile + link" << endl;
    cout <<"   -c   : convert to C and then compile + link" << endl;
    cout <<"   -asm : convert to asm for nasm and then compile + link" << endl;
    cout << endl;

    return 1;
  }
 
  bool show_tree = false;
  bool asm_compile = false;
  bool c_compile = false;
  bool cpp_compile = false;

  if (string(arg[argpos]) == "-ast"){
    show_tree = true;
    argpos++;
  }


  if (string(arg[argpos]) == "-cpp"){
    cpp_compile = true;
    argpos++;
  }

  if (string(arg[argpos]) == "-c"){
    c_compile = true;
    argpos++;
  }

  if (string(arg[argpos]) == "-asm") {
    asm_compile = true; 
    argpos++;
  }


  string script_file = string(arg[argpos]);
  ifstream in(script_file.c_str());
  if( !in.is_open () ){
    cerr << "could not open file: " << script_file <<endl;
    return 1;
  }

  Parser parser(in);
  
  if( parser.parse() ){
    TreeNode* root=parser.getTree();

    if (show_tree) {
      cout << "============================( AST )============================" << endl;
      root->showTree(root);
      cout << "---------------------------------------------------------------" << endl;
    }

    if(asm_compile){
      // create an nasm compiler instance
      cout << "generating asm code and linking..." << endl;
      string asm_file = basename(script_file) + ".asm";

      AsmCompiler compiler(root);
      compiler.generate("output.asm");
      compiler.link("output.asm");
    }
    else if(c_compile){
      cout << "generating c code and linking..." << endl;
      string c_file = basename(script_file) + ".c";

      CCompiler compiler(root);
      compiler.generate(c_file);
      compiler.link(c_file);
    }
    else if(cpp_compile){
      cout << "generating c++ code and linking..." << endl;
      string cpp_file = basename(script_file) + ".cpp";

      CppCompiler compiler(root);
      compiler.generate(cpp_file);
      compiler.link(cpp_file);
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


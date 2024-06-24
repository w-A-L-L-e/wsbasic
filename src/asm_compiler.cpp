/*=============================================================================
author        : Walter Schreppers
filename      : asm_compiler.cpp
description   : Compiles a parsetree into raw assymbly for nasm
bugreport(log): Work in progress. take what we've done in compiler_poc and use
that here. Later on we might just use LLVM instead which would be more performant
and easier to maintain. However for being minimalistic its cool to generate our own
asm and use nasm to create a binary here
=============================================================================*/

#include "asm_compiler.h"
#include <stdlib.h>

using namespace std;

AsmCompiler::AsmCompiler(TreeNode* tree){
  this->tree = tree;
  functionTable.clear();
  bBreak=false;
  bReturn=false;
}

// void AsmCompiler::run(){
//   bBreak=false;
//   bReturn=false;
//   
//   // compile program block
//   TreeNode* node;
//   TreeNode::const_iterator i;
//   for(i = tree->begin() ; i != tree->end(); ++i ){
// 
//     node=*i;
//     if( node->getType() == blockNode){
//       symtable main;
//       symbolTables.push( main ); //new symbol table for main block
//       compile( node );  //compile into asm
//       symbolTables.pop(); //free up stack
//       
//     }
//     else if( node->getType() == functionNode ){
//       string funcname=node->firstChild()->getName();
//       functionTable[ funcname ] = node; //store for later use
//     }
// 
//   }
// }

// https://steemit.com/programming/@orangeflash81/dynamic-memory-allocation-on-windows-in-nasm
void AsmCompiler::generate(const string& outfile){
  ofstream out(outfile);

  
  // define start which will be the main starting point like main() for c
  out << "global start"    << std::endl;
  out << "section .text"   << std::endl;
  out << "start:"          << std::endl;

  // TODO: this below code should actually be generated using compile(root)...
  // code for a println
  out << "      mov     rax, 0x2000004 ; write or println" << std::endl;
  out << "      mov     rdi, 1 ; stdout" << std::endl;
  out << "      mov     rsi, msg" << std::endl;  // msg is to be a label we'll need to generate that and also put it into data section
  out << "      mov     rdx, msg.len" << std::endl;
  out << "      syscall" << std::endl;

  // code for exit(25);
  out << "   mov rax, 0x2000001" << std::endl; // exit syscall
  out << "   mov rdi, " << 42 << std::endl;    // exit value
  out << "   syscall" << std::endl;            // jump to syscall (that was specified in rax above and has param rdi

  // data section to store all strings (our compiler will need to do this by looking at all string tokens in our .bas out
  out << "section .data"                            << std::endl;
  out << "  msg:    db      \"Hello, world!\", 10"  << std::endl;  // (ascii 10==new line)
  out << "  .len:   equ     $ - msg"                << std::endl;  // nasm trick to compute len of msg

  out.close();
}

void AsmCompiler::link(const string& asmfile){
  // TODO: output file = asmfile without .asm extension
  // assemble and link commands (this works on macos ventura)
  string asm_command = "nasm -f macho64 " + asmfile + " -o output.o";
  int res;
  res = system(asm_command.c_str());
  if (res != 0) std::cerr << "assembly failed" << endl;

  res = system("ld -arch x86_64 -macos_version_min 10.10.0 -static output.o -o output");
  if (res != 0) std::cerr << "linking failed" << endl;

  cout << "saved executable 'output'" << endl;
}


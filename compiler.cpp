/*=============================================================================
author        : Walter Schreppers
filename      : compiler.cpp
description   : Proof of concept to make a compiler class that outputs strait
                asm code and uses nasm to compile into an actual macos binary.
                
                Ideally we should use LLVM and just pass our parsetree into
                llvm intermediate language and then let it do its optimising.

                But for a lightweight compiler that generates real binary exe
                this will work also. And its something that can be done on 
                ancient hardware like amiga or some embedded device...
=============================================================================*/

#include <iostream>
#include <fstream>

int main(){

  std::fstream file("out.asm", std::ios::out);

  /* wsbasic pseudo code:
    println("Hello world!")
    exit(42)
  */

  // define start which will be the main starting point like main() for c
  file << "global start"    << std::endl;
  file << "section .text"   << std::endl;
  file << "start:"          << std::endl;

  // code for a println
  file << "      mov     rax, 0x2000004 ; write or println" << std::endl;
  file << "      mov     rdi, 1 ; stdout" << std::endl;
  file << "      mov     rsi, msg" << std::endl;  // msg is to be a label we'll need to generate that and also put it into data section
  file << "      mov     rdx, msg.len" << std::endl;
  file << "      syscall" << std::endl;

  // code for exit(25);
  file << "   mov rax, 0x2000001" << std::endl; // exit syscall
  file << "   mov rdi, " << 42 << std::endl;    // exit value
  file << "   syscall" << std::endl;            // jump to syscall (that was specified in rax above and has param rdi

  // data section to store all strings (our compiler will need to do this by looking at all string tokens in our .bas file
  file << "section .data"                            << std::endl;
  file << "  msg:    db      \"Hello, world!\", 10"  << std::endl;  // 10 is our \n here
  file << "  .len:   equ     $ - msg"                << std::endl;  // nasm trick to compute len of msg

  
  // TODO: for variables we can do stuff like
  // push value 42 on stack:
  // mov rax, 42
  // push rax 
  //
  // retrieve 42 from stack and put into rdi
  // ...
  // pop rdi 
  // syscall 
  // ....

  // use nasm to convert asm to object file
  // use ld to link it into an exe
  // brew install nasm

  // for linux binary
  // system("nasm -felf64 out.asm"); // on linux use elf format
  // system("ld -o out out.o");

  // for macos 10.10 binary (works on ventura)
  system("nasm -f macho64 out.asm");
  system("ld -arch x86_64 -macos_version_min 10.10.0 -static out.o -o out");

  std::cout << "succesfully compiled binary: 'out'" << std::endl;
  exit(0);
}





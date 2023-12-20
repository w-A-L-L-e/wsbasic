// The magic sauce here is to use extern "C" so function names dont get mangled by c++ compilers
// Author: Walter Schreppers

#include <iostream>

extern "C"
void execute(void){
  std::cout << "execute method was called" << std::endl;
}


extern "C"
bool add(int a, int b){
  std::cout << "a+b = "<< a+b << std::endl;
  if ((a+b) > 5) return true;
  else return false;
}



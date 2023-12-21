// The magic sauce here is to use extern "C" so function names dont get mangled by c++ compilers
// Author: Walter Schreppers

#include <iostream>

extern "C"
void plugin_execute(void){
  std::cout << "execute method was called" << std::endl;
}

extern "C"
bool plugin_add(int a, int b){
  std::cout << "a+b = "<< a+b << std::endl;
  if ((a+b) > 5) return true;
  else return false;
}

extern "C"
int plugin_sub(int a, int b){
  std::cout << "a-b = "<< a-b << std::endl;
  return a-b;
}



#include <iostream>

extern "C"
void execute(void){
  std::cout << "> execute method was called" << std::endl;
}


extern "C"
bool add(int a, int b){
  std::cout << "> a+b = "<< a+b << std::endl;
  if ((a+b) > 5) return true;
  else return false;
}



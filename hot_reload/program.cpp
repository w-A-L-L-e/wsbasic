// Description:
// example on how to hot reload a shared library in a running executable
// you can remove the .so file and even build a new one and code live updates
// The 2 hard parts in c++ is you need a cast of the void pointer and you need to add
// extern "C" in your functions otherwise names will get mangled (gcc does not need this)
//
// Author: Walter Schreppers
// 
#include <unistd.h> // for sleep
#include <iostream>
#include "plugin.h"


int main(){
  std::cout << "now just change plugin.cpp and run make. it will automatically hot reload the code!" << std::endl;
  int counter = 0;
  while(true){
    usleep(2000000);
    std::cout << std::endl << counter++ << std::endl;
    if (!load_library("libplugin.so")) continue;

    plugin_execute();
    bool res = plugin_add(5, 2);
    std::cout << "returned from add=" << res << std::endl;
    plugin_sub(5,2);

  }
}

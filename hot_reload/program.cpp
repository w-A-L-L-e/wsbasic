// Description:
// example on how to hot reload a shared library in a running executable
// you can remove the .so file and even build a new one and code live updates
// The 2 hard parts in c++ is you need a cast of the void pointer and you need to add
// extern "C" in your functions otherwise names will get mangled (gcc does not need this)
//
// Author: Walter Schreppers



#include <unistd.h> // for sleep
#include <dlfcn.h>  // for dlopen
#include <iostream>
#include "plugin.h"


void *libplug = NULL;

// unfonrtunately in clang on macos the name##_t and #name to get string and preprocessor concat
// does not work here
#define METHOD(name) name##_t name = NULL; 
PLUGIN_METHODS
#undef METHOD

// above macor generates things like this:
// plugin_add_t plug_add = NULL;

bool loadlib(const char* plugin_file){
  if (libplug != NULL) dlclose(libplug);

  // libplug = dlopen(plugin_file, RTLD_LAZY);
  libplug = dlopen(plugin_file, RTLD_NOW);
  if (libplug == NULL) {
    std::cout << "ERROR could not load so file "<<plugin_file<<" "<<dlerror()<<std::endl;
    return false;
  }


  void *fhandle;
  #define METHOD(name) \
    fhandle = dlsym(libplug, #name); \
    if (name == NULL) { \
      std::cout << "ERROR could not find "<< #name << "in "<<plugin_file<<" "<<dlerror()<<std::endl; \
      return false; \
    } \
    name = reinterpret_cast<name##_t>(reinterpret_cast<long>(fhandle));
  PLUGIN_METHODS
  #undef METHOD

  /*
  how code looks without above x macro above:
  fhandle = dlsym(libplug, "add");
  if (fhandle == NULL) {
    std::cout << "ERROR could not find method in "<<plugin_file<<" "<<dlerror()<<std::endl;
    return false;
  }

  // cast to correct typed function pointer
  plug_add = reinterpret_cast<plugin_add_t>(reinterpret_cast<long>(fhandle)) ;
  */

  return true;
}


int main(){
  std::cout << "now just change plugin.cpp and run make. it will automatically hot reload the code!" << std::endl;
  int counter = 0;
  while(true){
    usleep(2000000);
    std::cout << std::endl << counter++ << std::endl;
    if (!loadlib("libplugin.so")) continue;

    plugin_execute();
    bool res = plugin_add(5, 2);
    std::cout << "returned from add=" << res << std::endl;
    plugin_sub(5,2);

  }
}

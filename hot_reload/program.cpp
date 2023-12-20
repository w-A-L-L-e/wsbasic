// example on how to hot reload a shared library in a running executable
// you can remove the .so file and even build a new one and code live updates

#include <unistd.h> // for sleep
#include <dlfcn.h>  // for dlopen
#include <iostream>
#include "plugin.h"


const char* plugin_file = "libplugin.so";
void *libplug = NULL;
plugin_execute_t plug_execute = NULL;
plugin_add_t plug_add = NULL;

bool loadlib(const char* plugin_file){
  if (libplug != NULL) dlclose(libplug);

  // libplug = dlopen(plugin_file, RTLD_NOW);
  libplug = dlopen(plugin_file, RTLD_LAZY);
  if (libplug == NULL) {
    std::cout << "ERROR could not load so file "<<plugin_file<<" "<<dlerror()<<std::endl;
    return false;
  }

  void *fhandle = dlsym(libplug, "execute");
  if (fhandle == NULL) {
    std::cout << "ERROR could not find method in "<<plugin_file<<" "<<dlerror()<<std::endl;
    return false;
  }

  // cast to correct typed function pointer
  plug_execute = reinterpret_cast<plugin_execute_t>(reinterpret_cast<long>(fhandle)) ;


  fhandle = dlsym(libplug, "add");
  if (fhandle == NULL) {
    std::cout << "ERROR could not find method in "<<plugin_file<<" "<<dlerror()<<std::endl;
    return false;
  }

  // cast to correct typed function pointer
  plug_add = reinterpret_cast<plugin_add_t>(reinterpret_cast<long>(fhandle)) ;

  return true;
}


int main(){
  std::cout << "now just change plugin.cpp and run make. it will automatically hot reload the code!" << std::endl;
  int counter = 0;
  while(true){
    usleep(2000000);
    std::cout << std::endl << counter++ << std::endl;
    if (!loadlib(plugin_file)) continue;

    plug_execute();
    bool res = plug_add(5, 2);
    std::cout << "returned from add=" << res << std::endl;

  }
}

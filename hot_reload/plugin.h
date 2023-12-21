// some demo functions we call from program.cpp and they will live reload whenever re-compiled
// Author: Walter Schreppers
// UPDATE we use x macro trick seen online


#ifndef PLUGIN_H
#define PLUGIN_H

#include <dlfcn.h>  // for dlopen
#include <iostream>

// we define the methods as an x macro to code generate the loading and pointers in program.cpp
// example:
//   typedef bool (*plugin_add_t)(int a, int b);
// typedef_name,           method name,     func string,  return param, function params
#define PLUGIN_METHODS \
  METHOD(plugin_execute_t, plugin_execute,  "plugin_execute", void,  void)  \
  METHOD(plugin_add_t,     plugin_add,      "plugin_add",     bool,  int, int) \
  METHOD(plugin_sub_t,     plugin_sub,      "plugin_sub",     int,   int, int)      


// generate typedef statements 
// example: 
//   typedef void (*plugin_execute_t)(void);
#define METHOD(name_t, name, name_str, ret, ...) typedef ret (*name_t)(__VA_ARGS__);
PLUGIN_METHODS
#undef METHOD

// generate function pointers
// example:
//   plugin_add_t plug_add = NULL;
#define METHOD(name_t, name, name_str, ret, ...) name_t name = NULL;
PLUGIN_METHODS
#undef METHOD


// keep track of plugin library in libplug global here
void *libplug = NULL;

bool loadlib(const char* plugin_file){
  if (libplug != NULL) dlclose(libplug);

  // libplug = dlopen(plugin_file, RTLD_LAZY);
  libplug = dlopen(plugin_file, RTLD_NOW);
  if (libplug == NULL) {
    std::cout << "ERROR could not load so file "<<plugin_file<<" "<<dlerror()<<std::endl;
    return false;
  }

  #define METHOD(name_t, name, name_str, ret, ...) \
    name = reinterpret_cast<name_t>(reinterpret_cast<long>(dlsym(libplug, name_str))); \
    if (name == NULL) { \
      std::cout << "ERROR could not find method "<<plugin_file<<" "<<dlerror()<<std::endl; \
      return false; \
    } 
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


#endif

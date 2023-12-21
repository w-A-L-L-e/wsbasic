// some demo functions we call from program.cpp and they will live reload whenever re-compiled
// Author: Walter Schreppers
// UPDATE we use x macro trick seen online


#ifndef PLUGIN_H
#define PLUGIN_H

// these typedefs allow to cast our void pointers later
// typedef void (*plugin_execute_t)(void);
// typedef bool (*plugin_add_t)(int a, int b);
// typedef int (*plugin_sub_t)(int a, int b);

// we define the methods as an x macro to code generate the loading and pointers in program.cpp
#define PLUGIN_METHODS    \
  METHOD(plugin_execute_type, plugin_execute, "plugin_execute" void, void)  \
  METHOD(plugin_add_type, plugin_add, "plugin_add", bool, int, int)      \
  METHOD(plugin_sub_type, plugin_sub, "plugin_sub", int, int, int)      


#define METHOD(name_t, name, ret, ...) typedef (name_t)(__VA_ARGS__)
PLUGIN_METHODS
#undef

#endif

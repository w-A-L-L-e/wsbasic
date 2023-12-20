// some demo functions we call from program.cpp and they will live reload whenever re-compiled
// Author: Walter Schreppers


#ifndef PLUGIN_H
#define PLUGIN_H

// these typedefs allow to cast our void pointers later
typedef void (*plugin_execute_t)(void);
typedef bool (*plugin_add_t)(int a, int b);

#endif

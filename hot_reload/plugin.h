#ifndef PLUGIN_H
#define PLUGIN_H

//void execute();
typedef void (*plugin_execute_t)(void);
typedef bool (*plugin_add_t)(int a, int b);

#endif

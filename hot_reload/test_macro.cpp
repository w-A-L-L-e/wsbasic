#define PLUGIN_METHODS    \
  METHOD(plugin_execute)  \
  METHOD(plugin_add)      \
  METHOD(plugin_sub)      
       

#define STR(X) #X

#define METHOD(name) name" "_t name= NULL; 
PLUGIN_METHODS
#undef METHOD


#define METHOD(name) \
    void *name##_handle; \
    name##_handle = dlsym(libplug, #name); \
    if (name##_handle == NULL) { \
      std::cout << "ERROR could not find "<< #name << "in " << plugin_file << " " << dlerror()<<std::endl; \
      return false; \
    } \
    name = reinterpret_cast<name##_t>(reinterpret_cast<long>(name##_handle));
  PLUGIN_METHODS
#undef METHOD


#define CONCAT(p1,p2)  p1##p2

CONCAT(hello,world)
// run cpp test_macro.cpp to see output (this works on linux, not on macox clang however)

CXX         = g++ 
FLAGS       = -O2 -Wall -fno-strict-aliasing

all: program libplugin.so

program: program.cpp
	$(CXX) $(CXXFLAGS) program.cpp -o program   # -ldl is optional on macos

libplugin.so: plugin.cpp
	$(CXX) $(CXXFLAGS) -fPIC -shared plugin.cpp -o libplugin.so

run:
	LD_LIBRARY_PATH="." ./program    

clean:
	@rm -vf program libplugin.so

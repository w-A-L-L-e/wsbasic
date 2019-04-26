
WSBasic is a BASIC interpreter written in C++ using recursive descent OO techniques instead of the outdated lex/yacc tools. It can already be used as a scripting replacement for bash and was also used as basis for kturtle's logo interpreter. Can act as a replacement for bash or sh scripting.

Originally released on sourceforge and freecode.com over 19 years ago (april 2000):
Sourceforge pages: http://wsbasic.sourceforge.net/ and https://sourceforge.net/projects/wsbasic/
Freecode pages: http://freshmeat.sourceforge.net/projects/wsbasic 


Later forked and used by kturtle project of KDE:
https://kde.org/applications/education/kturtle/

Cleaned up and made to compile again on latest Xcode in 2019:
```
gcc --version
Configured with: --prefix=/Applications/Xcode.app/Contents/Developer/usr --with-gxx-include-dir=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/c++/4.2.1
Apple LLVM version 10.0.1 (clang-1001.0.46.4)
Target: x86_64-apple-darwin18.5.0
Thread model: posix
InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin

```

Most likely still compiles on linux with regular gcc and on windows using mingw ;).



Compiling:

```
➜  wsbasic git:(master) make
g++  -O2 -Wall -c number.cpp
g++  -O2 -Wall -c lexer.cpp
g++  -O2 -Wall -c parser.cpp
g++  -O2 -Wall -c treenode.cpp
g++  -O2 -Wall -c executer.cpp
g++  -O2 -Wall main.cpp -o wsbasic lexer.o parser.o treenode.o executer.o number.o
```


Running some test scripts:
```
➜  wsbasic git:(master) ✗ ./tests.sh 
2.000000
b=20.000000
c=11.000000
tricky=0.900000
b1=1.000000
b2=0.000000
b3=1.000000
g=-6.000000
1.000000.	total 752
2.000000.	drwxr-xr-x  27 wschrep  staff     864 Apr 26 17:12 .
3.000000.	drwxr-xr-x  89 wschrep  staff    2848 Apr 26 16:43 ..
4.000000.	drwxr-xr-x  13 wschrep  staff     416 Apr 26 17:12 .git

.... some more output and input requested here (basically runs all scripts in scripts dir)
```


Executing a single script, here we for example compute some prime numbers:
```
./wsbasic scripts/primes.b 
```

Nowadays you can still learn from this but will most likely move on to LLVM tools to make yourself a neat compiler with minimal effort.
A good starting point is this tutorial:
https://llvm.org/docs/tutorial/LangImpl01.html

Still for minimal implementations (like on embedded on a microcontroller for instance having basic on a atmega or something) wsbasic is still 
a good way to go.

As stated in the original release this is a LGPL project but requires a mention of the original author:
So enjoy and feel to modify + reuse and even use in a commercial product. But do remember to include a reference to the original author 
being Walter Schreppers. Any changes to the sources are encouraged to be released according to LGPL.

And watch out for this Gotcha: KTurtle is even stricter by using GPL and not LGPL, also something I frown upon now many years later when I see more importance in choosing licenses. 
Basically no permission was asked to myself to make an even stricter license. One could argue that this project gives you a free pass and treat large parts of KTurtle as LGPL instead of
GPL because wsbasic has precedence. WSBasic was created to make it easier for other developers to create their own interpreters without any extra tools. By having a clean and simple
implementation as a reference it's a great starting point to make something more specific like KTurtle did.



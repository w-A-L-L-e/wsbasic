# WSBasic

WSBasic is a BASIC interpreter written in C++ using recursive descent OO techniques instead of the outdated lex/yacc tools. It can already be used as a scripting replacement for bash and was also used as basis for kturtle's logo interpreter. Can act as a replacement for bash or sh scripting.


## History
Originally released on sourceforge and freecode.com over 19 years ago (april 2000):
* Sourceforge pages: 
  - http://wsbasic.sourceforge.net/ 
  - https://sourceforge.net/projects/wsbasic/
* Freecode pages: 
  - http://freshmeat.sourceforge.net/projects/wsbasic 

Later forked and used by kturtle project of KDE:
  - https://kde.org/applications/education/kturtle/

Cleaned up and made to compile again on latest Xcode in 2019:
```
gcc --version
Configured with: --prefix=/Applications/Xcode.app/Contents/Developer/usr --with-gxx-include-dir=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/c++/4.2.1
Apple LLVM version 10.0.1 (clang-1001.0.46.4)
Target: x86_64-apple-darwin18.5.0
Thread model: posix
InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin

```


And watch out for this Gotcha: KTurtle is even stricter by using GPL and not LGPL, also something I frown upon now many years later when I see more importance in choosing licenses. 
Basically no permission was asked to myself to make an even stricter license. One could argue that this project gives you a free pass and treat large parts of KTurtle as LGPL instead of
GPL because wsbasic has precedence. WSBasic was created to make it easier for other developers to create their own interpreters without any extra tools. By having a clean and simple
implementation as a reference it's a great starting point to make something more specific like KTurtle did. And originally KTurtle also stated me as a co-author in the about box.

Somehow Cies decided to remove people as his project evolved from the about box and he ended up being the only one mentioned on wikipedia now:
https://en.wikipedia.org/wiki/KTurtle. 
It's a wiki that can evolve, and maybe someday the other authors will be added? One can only hope... right? ;)

Proof of other authers, including myself:
https://man.cx/kturtle 
https://github.com/KDE/kturtle/blob/master/AUTHORS

Original version showing my name and Anne-Marie Mahfouf in about box:
https://walter.schreppers.com/files/kturtle.gif

Without further ado: Just compile this project and run some scripts form commandline and enjoy reading, learning and implementing your own compilers by playing
with a working example:

## Compiling:

```
➜  wsbasic git:(master) make
g++  -O2 -Wall -c number.cpp
g++  -O2 -Wall -c lexer.cpp
g++  -O2 -Wall -c parser.cpp
g++  -O2 -Wall -c treenode.cpp
g++  -O2 -Wall -c executer.cpp
g++  -O2 -Wall main.cpp -o wsbasic lexer.o parser.o treenode.o executer.o number.o
```

Same applies for compiling on linux, windows with mingw. Feel free to ask if you get some error or warning on those platforms.

### The implemented basic language

Someone already started documenting it in the past here http://wsbasic.sourceforge.net/. But I've got no spare time to do it myself. Currently too busy with work, family 
and even a kickstarter project https://www.kickstarter.com/projects/715415099/anykey-the-usb-password-key

Basically you've got something turing complete and that resembles the basic language from C64 but without line numbers and gotos but functions instead (so a bit more modern).
String manipulations, input, reading+writing files are already possible. 
Feel free to document and submit a pull request.

Even back then making something that just worked was more important than documenting the BNF also anyone feel free to complete and submit a PR ;).
Here's a start as seen in parser.cpp, feel free to complete the comments:
```
BNF for arithmetic expressions (improved unary minus)

    <expression>    ::= <term> [<addop> <term>]*
    <term>          ::= <signed factor> [<mulop> <signed factor>]*
    <signed factor> ::= [<addop>] <factor>
    <factor>        ::= <integer> | <variable> | (<expression>)

```
But rest assured the actual implementation is complete and has a full language implemented with all basic features, loops, expressions, function calls, the works.
And the entire parser class is less than 800 lines of c++ code so readability and simplicity is key here ;).
Same for the lexer and execution classes, keeping the implementation as tight and simple as possible while still providing a fully working interpreter that can even
be extended to a compiler by adding 1 more class (a compiler class that iterates the N-tree similarly to Executer but now just spits out code with cout statements would be enough to do that in the same minimal philosophy )

### Usage and examples

Try running some test scripts:
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


## Authors
Walter Schreppers


## License
This project is released under LGPL.


### Note on license
As stated in the original release, this is a LGPL project but with a small addition that it requires a mention of the original author.
So enjoy and feel to modify + reuse and even use in a commercial product. But do remember to include a reference to the original author 
being Walter Schreppers. Any changes to the sources are encouraged to be released as LGPL.


update on 24/1/2020

Made some changes now the main block does not need begin/end and function declarations need def.

Example:

```
def showName( name )
begin
  println( "My name is "+ name )
end

println("here is the main block running now")
showName( "Walter")
```

Running example:
```
➜  wsbasic git:(master) ✗ ./wsbasic scripts/hello.b
here is the main block running now
My name is Walter
```



This makes the hello world also much shorter:

```
println("Hello world")
```

Running hello world:
```
➜  wsbasic git:(master) ✗ ./wsbasic scripts/hello_world.b 
Hello world
```

Look in scripts dir for more elaborate examples. It's pretty much got everything like bash has to offer and more...


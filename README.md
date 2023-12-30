# WSBasic

WSBasic is a BASIC interpreter written in C++ using recursive descent OO techniques instead of the outdated lex/yacc tools. It can already be used as a scripting replacement for bash and was also used as basis for kturtle's logo interpreter. Can act as a replacement for bash or sh scripting.
Added some ansi and unicode support that allowed to render this nice mandelbrot also on 30/12/2023.
<img width="1141" alt="Screenshot 2023-12-30 at 19 19 36" src="https://github.com/w-A-L-L-e/wsbasic/assets/710803/1083ad64-b897-47b5-9477-a099b0339a5f">


## Compiling wsbasic
Just run make and it should generate an executable wsbasic and a static libwsbasic.a in lib folder.
On macos you need Xcode developer tools installed for this.

```
$ make
g++  -O2 -Wall -g  -c src/lexer.cpp -o src/lexer.o
g++  -O2 -Wall -g  -c src/parser.cpp -o src/parser.o
...
g++  -O2 -Wall -g  -o wsbasic src/main.o src/var.o src/lexer.o src/parser.o src/treenode.o src/executer.o src/asm_compiler.o src/c_compiler.o src/cpp_compiler.o src/string_utils.o 
```

Same applies for compiling on linux, windows with mingw. Feel free to ask if you get some error or warning on those platforms.


## Usage

Run without arguments and get usage:
```
$ ./wsbasic
Usage: ./wsbasic [-ast, -cpp, -asm] <WSBASIC SCRIPT> 
   -ast : show parse result in absact syntax tree for debugging
   -cpp : convert to cpp and then compile + link
   -asm : convert to asm for nasm and then compile + link
```

```
$ cat hello.bas
x=40
println("Hello world!")
exit(x+2)
```

Just running above script directly:

```
./wsbasic hello.bas
Hello world!
```

Show the AST after parsing and use assembly to create executable:
```
./wsbasic -ast -asm hello.bas
============================( AST )============================
- main block (8) 
  - assignment (15) 
    - x (17) 
    - constant = 40 (18) 
  - println (13) 
    - const str = Hello world! (19) 
    - newline (19) 
  - exit (5) 
    - add (20) 
      - x (17) 
      - constant = 2 (18) 
---------------------------------------------------------------
generating asm code and linking...
saved executable 'output'
```

We can run the generated executable (in future version it will be called hello.exe here, right now its always output.o and output)
```
$ ./output
Hello, world!
```

We can check the exit statement properly set the right return code in bash:
```
$ echo $?
42
```


We can even use things like we do in bash. Here's a nice example of fetching your ip address from the internet.
```
$ cat example.bas

#!/usr/local/bin/wsbasic

def make_webrequest()
begin
  url =  "https://ipinfo.io/ip"   
  result  = run("curl -s " + url)
  println "your public ip = ", result
end

make_webrequest()

println "simple forloop example"
for i=1 to 5
  println i

```

When we run this example curl will be used and the output is available in our basic script to do anything we want:
```
$ ./wsbasic example.bas

your public ip = 94.226.168.242
simple forloop example
1.000000
2.000000
3.000000
4.000000
5.000000
```

More script examples can be found in the scripts dir.
```
$ cd scripts
$ ./tests
```


## History
Originally released on sourceforge and freecode.com over 19 years ago (april 2000):
* Sourceforge pages: 
  - http://wsbasic.sourceforge.net/ 
  - https://sourceforge.net/projects/wsbasic/
* Freecode pages: 
  - http://freshmeat.sourceforge.net/projects/wsbasic 

Later forked and used by kturtle project of KDE:
  - https://kde.org/applications/education/kturtle/

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


### Compiling scripts to C++

The ASM and C versions of compilers are more in the proof of concept stage. However the CppCompiler is pretty much just as capable
as the executor (interpreting). For cpp compiler only a couple more todos: functions with return, for loops and substr need implementing. Apart from that we tried already a little benchmark to compute primes using while loop and compiling definitately gives around a 400% boost in speed. We haven't optimised at all to reduce object copies etc. So there is even more low hanging fruit here.
Also in 2023 we rewrote the Var class which already made the executor about 3 times faster so in reality vs old wsbasic executor
when compiling you get a factor 7 to 10 times faster performance. It's actually faster than python now ;).

```
$ cat benchmark.b
#!/usr/local/bin/wsbasic

i=2
while(i < 5000)
begin
  prime = 1
  j=2
  while j < (i-1)
  begin
    if( (i % j) == 0 )
    begin
      prime=0
    end
    j=j+1
  end

  if prime == 1 
  begin
    print(i)
    print(" ")
  end

  i=i+1
end

println("\ndone")
```

Execution using interpreting:
```
 time ./wsbasic benchmark.b 
2.000000 3.000000 5.000000 7.000000 11.000000 13.000000 17.000000 19.000000 23.000000 ...
./wsbasic benchmark.b  5.30s user 0.09s system 98% cpu 5.474 total
```

Compiling our benchmark.b to c++:
```
 ./wsbasic -cpp benchmark.b
generating c++ code and linking...
saved executable 'benchmark'
```

The generated c++ output is readable but already shows room for improvements.
```
#include "benchmark.h"
#include <iostream>
#include <string>

int main() {
  std::cout << std::fixed;
  Var i = Var(2.000000);
  while (i < Var(5000.000000)) {
    Var prime = Var(1.000000);
    Var j = Var(2.000000);
    while (j < i - Var(1.000000)) {
      if (i % j == Var(0.000000)) {
        prime = Var(0.000000);
      }

      j = j + Var(1.000000);
    }

    if (prime == Var(1.000000)) {
      std::cout << i;
      std::cout << " ";
    }

    i = i + Var(1.000000);
  }

  std::cout << "\ndone" << std::endl;
  return 0;
}
```

Run our generated native exe we get exact same output but it runs a lot faster:
```
time ./benchmark
2.000000 3.000000 5.000000 7.000000 11.000000 13.000000 17.000000 19.000000 23.000000 ...

./benchmark  1.01s user 0.01s system 99% cpu 1.029 total
```

Rewrote the benchmark to python manually and running it there shows the latest python interpreter is a little faster
than wsbasic interpreting mode.

```
$ python --version
Python 3.11.7

$ time python benchmark.py

2 3 5 7 11 13 17 19 23 ... 

python benchmark.py  3.95s user 0.12s system 94% cpu 4.309 total
```

And also python is working with integers here and wsbasic does not use those yet (all is casted to double).
So most likely the wsbasic executor will be around same speed when we add integere support. But even with only double as types
the compiled version beats python speed by factor of 4 already (1.01 sec vs 3.95 with python). Also our interprete mode
is actually not that bad (but for sure could be improved), we have 5.3 seconds using doubles with wsbasic in interprete mode vs python's 3.95.


The generated benchmark.h now only contains the include file but if you write methods in your basic scripts
their implementation will be compiled into the header.

You can test this using the compiler_test.b script
```
./wsbasic -cpp compiler_test.b
```


## UPDATES

Cleaned up and made to compile again on latest Xcode in 2019. 
24/1/2020: Made some changes now the main block does not need begin/end and function declarations need def.

Functions example:

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

19/12/2023: Shortly revisited this for a few hours in and started on a compiler class that would generate asm to also support compiling into a binary executable. Did a little proof of concept to see it would work with nasm after seeing some youtube video that sparked my interest. 

### Compiling scripts ASM and C versions
Compiling to binary is just in proof of concept fase right now, might revisit this and complete it somewhere in 2024 if I have time. However
as poc it shows it would be pretty easy to get most features up and running quite quickly. Using llvm would however make it much more performant
than doing our simple asm generation here. But not relying on llvm does allow this to be ported to a low resource embedded system or an amiga or something like that :D

Example of generating assembly and then link this into standalone executable:
```
 ./wsbasic -a hello.bas
generating asm code and linking...
saved executable 'output'

$ cat output.asm
global start
section .text
start:
      mov     rax, 0x2000004 ; write or println
      mov     rdi, 1 ; stdout
      mov     rsi, msg
      mov     rdx, msg.len
      syscall
   mov rax, 0x2000001
   mov rdi, 42
   syscall
section .data
  msg:    db      "Hello, world! (generated with ASM)", 10
  .len:   equ     $ - msg

$ ./output
Hello, world! (generated with ASM)
```

Example of generating c from basic and then linking into a standalone executable:
```
$ ./wsbasic -c hello.bas
generating c code and linking...
saved executable 'output'

$ ./output
Hello world! (generated by using cpp file)

$ cat output.cpp ...
```


### Future work

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

Look in scripts dir for more elaborate examples. It's pretty much got everything like bash has to offer and more...



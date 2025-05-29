format ELF64 executable 3

; format ELF64 executable 3
entry start
extrn printf

define SYS_exit     60
define SYS_write    1

define STDOUT       1
define exit_success 0

segment readable executable
start:
  mov rax, SYS_write
  mov rdi, STDOUT 
  mov rsi, hello
  call printf

  mov rax, SYS_exit  ; 60 == SYS_EXIT
  mov rdi, exit_success
  syscall

segment readable writable
hello: db "Hello, World", 10

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

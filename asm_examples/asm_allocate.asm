BITS 64
DEFAULT REL ; RIP-relative addressing by default
;
; Basic OS X calls to glibc
;
; compile with:
; nasm -g -f macho64 malloc64.asm
; gcc -o a.out malloc64.o
;

; glibc stuff
extern _puts, _printf, _malloc, _free

; static data
section .data

hello_world_str db "Hello world!", 10, 0
int_str db "Address %llx", 10, 0

; code
section .text

global _main

_main:
        ; save registers and align stack
        push rbp
        push r12
        push rbx

        lea  rdi, [hello_world_str]
        call _puts

        mov  rdi, 26
        call _malloc

        ; check if the malloc failed
        test rax, rax
        jz   fail_exit
        mov  rbx, rax

        xor  rax, rax
        mov  rsi, rbx
        lea  rdi, [int_str]
        call _printf

        ; print "A\nB\n..."
        mov [rbx], word 0xD41 ; 'A\n'

        mov r12, 20
_loop:
        mov  rdi, rbx
        call  _puts
        inc  qword [rbx]
        dec  r12
        jnz  _loop

        ; free the malloc'd memory
        mov  rdi, rbx
        call _free

        xor rax, rax
        pop rbx
        pop r12
        pop rbp
        ret

fail_exit:
        mov rax, 1
        pop rbx
        pop r12
        pop rbp
        ret


 global _start
    section .text
_start:
print_prompt: 
    ;mov rax, 4                  ; syscall write
    mov rax, 0x02000004  
    mov rdi, 1                  ; stdout
    mov rsi, prompt             ; prompt string
    mov rdx, 2                  ; 2 bytes
    syscall
    mov r8, 0                   ; length of buffer
    mov rsi, buffer             ; initialize buffer location
    mov r10, 0                  ; reset EOF flag
read_char:
    mov rax, 0x02000002         ; read syscall
    mov rdi, 0                  ; stdin
    mov rdx, 1                  ; 1 byte
    syscall
    cmp rax, 0                  ; is EOF?
    jne non_eof                 ; jump to process char if not EOF (read=0)
    mov r10, 1                  ; flag EOF
    jmp print_word              ; print_word
non_eof:    
    inc r8                      ; increment buffer length
    inc rsi                     ; increment buffer location
    mov r9, 0                   ; clear r9
    mov r9b, byte [rsi-1]       ; move previous char into r9
    cmp r9, 10                  ; is it NEWLINE?
    je print_word               ; print the word if it is (including the NEWLINE)
    cmp r8, buffer_size         ; compare current length with buffer_size
    je print_word               ; print the word if it's equal to buffer_size
    jmp read_char               ; read next character
print_word:
    ; mov rax, 4                  ; syscall write
    mov rax, 0x02000004  
    mov rdi, 1                  ; stdout
    mov rsi, buffer             ; buffer
    mov rdx, r8                 ; length of word
    syscall
    cmp r10, 1                  ; is EOF?
    je exit
    jmp print_prompt
exit:   
    mov rax, 60                 ; system call for exit
    xor rdi, rdi                ; exit code 0
    syscall                     ; invoke operating system to exit

    section .bss
buffer_size equ 64    
buffer:
    resb buffer_size            ; reserve a 64-byte buffer
	
    section .data
prompt:
    dw "> "

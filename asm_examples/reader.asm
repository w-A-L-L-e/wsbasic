        global start
        default rel

        section .text
start:
        call print_question
        call read_input
        call print_answer
        call exit

print_question:
; print question message
        mov rax, syscall_write         ; write
        mov rdi, stdout                ; stdout
        mov rsi, m_question            ; msg text
        mov rdx, m_question.len        ; msg length
        syscall
        ret

print_answer:
; print response mensage
        mov rax, syscall_write         ; write
        mov rdi, stdout                ; stdout
        mov rsi, m_answer              ; msg text
        mov rdx, m_answer.len          ; msg length
        syscall

; print user input
        mov rax, syscall_write         ; write
        mov rdi, stdout                ; stdout
        mov rsi, num                   ; msg text
        mov rdx, 0xff                  ; msg length
        syscall
        ret

read_input:
; read user input
        mov rax, syscall_read          ; read
        mov rdi, stdin                 ; stdin
        mov rsi, num                   ; input bytes
        mov rdx, 0xff                  ; bytes to read
        syscall
        ret

exit:
; exit program
        mov rax, syscall_exit          ; exit
        mov rdi, 0                     ; code 0
        syscall

        section .data
        
        syscall_write equ 0x2000004
        syscall_read equ 0x2000003
        syscall_exit equ 0x2000001
        stdout equ 1
        stdin equ 0

m_question:
        db "What's your name? "
        .len equ $-m_question
m_answer:
        db 0x0A, "Thank you for answering, "
        .len equ $-m_answer

        section .bss
        num resb 0xff

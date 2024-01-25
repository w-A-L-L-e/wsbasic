        ;global _main
        global start
        default rel

        section	.text
; _main:
; call delete_file             ; delete file if exists
start:

        call create_file               ; create file using open()
        call open_file

        mov [fd_out], rax              ; store file descriptor of new file

        call write_file                ; write text to file

        call close_file

        call exit

delete_file:
; delete an existing file
        mov rax, sys_delete
        mov rdi, file_name             ; user_addr_t of path
        syscall
        ret

create_file:
; create file with writing permission (open file)
        mov rax, sys_open
        mov rdi, file_name             ; user_addr_t of path
        mov rsi, 0x0200                ; O_CREAT flag
        mov rdx, 644o                  ; mode -rw-r--r-- in octal value
        syscall
        ret

open_file:
; create file with writing permission (open file)
        mov rax, sys_open
        mov rdi, file_name             ; user_addr_t of path
        mov rsi, 0x0002                ; O_RDWR flag
        syscall
        ret

write_file:
; writes message to just created file
        mov rax, sys_write
        mov rdi, [fd_out]              ; opened file descriptor integer
        mov rsi, msg                   ; user_addr_t of message
        mov rdx, msg.len               ; user_addr_t of length
        syscall
        ret

close_file:
        mov rax, sys_close
        mov rdi, [fd_out]
        syscall
        ret

exit:
; exits program
        mov rax, sys_exit              ; system call number (sys_exit)
        syscall                        ; call kernel

        section	.data
        sys_exit equ 0x2000001
        sys_read equ 0x2000003
        sys_write equ 0x2000004
        sys_open equ 0x2000005
        sys_close equ 0x2000006
        sys_delete equ 0x20000E2

file_name:
        db 'myfile.txt', 0
msg:
        db 'Hello World', 0x0A
        .len equ $-msg

        section .bss
        fd_out resb 1


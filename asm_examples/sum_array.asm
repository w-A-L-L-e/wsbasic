; adapted from https://www.tutorialspoint.com/assembly_programming/assembly_arrays.htm

        global start                   ; must be declared for linker (ld)
        default rel

        section	.text
start:

        mov rax, 3                     ; number bytes to be summed
        mov rdi, 0                     ; rdi will store the sum
        mov rcx, x                     ; rcx will point to the current element to be summed

top:
        add rdi, [rcx]

        add rcx, 1                     ; move pointer to next element
        dec rax                        ; decrement counter
        jnz top                        ; if counter not 0, then loop again

done:

        add rdi, '0'
        mov [sum], rdi                 ; done, store result in "sum"

display:

        mov rax, syscall_write         ; system call number (sys_write)
        mov rdi, stdout                ; file descriptor (stdout)
        mov rsi, sum                   ; message to write
        mov rdx, 1                     ; message length

        syscall                        ; call kernel

        mov rax, syscall_exit          ; system call number (sys_exit)
        syscall                        ; call kernel

        section	.data

        syscall_write equ 0x2000004
        syscall_exit equ 0x2000001
        stdout equ 1

        global x
x:
        db 2
        db 4
        db 3

sum:
        db 0

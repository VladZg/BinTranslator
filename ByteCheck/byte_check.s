section .text
global main

main:
    push rbp
    mov rbp, rsp

    cmp r13d, r14d
    jg  1242
    jge 1242
    jl  1242
    jle 1242
    je  1242
    jne 1242

    mov rax, 0x01
    mov rdi, 1
    mov rsi, Msg
    mov rdx, MsgLen
    syscall

    pop rbp

    mov rax, 0x3C
    xor rdi, rdi
    syscall

section .data

Msg:    db  "Programm for check!", 0x0A
MsgLen: equ $ - Msg

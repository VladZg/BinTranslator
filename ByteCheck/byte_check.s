section .text
global main

main:
    push rsi
    push rdi

    push 13
    push rax
    pop rsi
    pop rdi
    sub rsi, rdi
    push rsi
    pop rdi

    pop rdi
    pop rsi

    mov rax, 0x01
    mov rdi, 1
    mov rsi, Msg
    mov rdx, MsgLen
    syscall

    mov rax, 0x3C
    xor rdi, rdi
    syscall

section .data

Msg:    db  "Programm for check!", 0x0A
MsgLen: equ $ - Msg

section .text
global main

main:
    push rbp
    mov rbp, rsp

    push 7000
    push 1500
    fild dword [rsp+8]
    fild qword [rsp]
    push dword 1000
    fidiv dword [rsp]
    fsqrt
    add rsp, 12
    fistp qword [rsp]

    mov rdi, 0x4635637454352787

    mov rax, r14
    cmp r13, r14

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

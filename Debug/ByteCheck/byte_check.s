section .text
global main

main:
    push rbp
    mov rbp, rsp

    finit
    sub rsp, 800
    mov rbp, rsp
    add rsp, 800

    push 7000
    push 1500
    fild dword [rsp+8]
    fild qword [rsp]
    push dword 1000
    fidiv dword [rsp]
    fsqrt
    add rsp, 800
    fistp qword [rsp]

    mov r13, qword [r14 + r13 * 8]

    pop  r13
    mov  r14, qword [rbp + r13 * 8]
    push r14

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

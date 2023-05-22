main:
{
    in
    pop rbx
    push 1
    pop rax

    push rbx
    push 666
    je exit

    call factorial

    push rax
    out

    jmp main

exit:
    hlt
}

factorial:
{
    push rbx
    push 1
    jae correct_num
    push 0
    pop rax
    jmp exit_factorial

correct_num:
    push rbx
    push 1
    jne continue_factorial
    jmp exit_factorial

continue_factorial:
    push rbx
    push rax
    mul
    pop rax

    push -1 + rbx
    pop rbx
    call factorial

exit_factorial:
    ret
}

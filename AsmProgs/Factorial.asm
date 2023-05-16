main:
{
start:
    in
    pop rbx

    push rbx
    push 0
    je finish

    push 1
    pop rcx
    call factorial
    print rcx
    dump full

    jmp start

finish:
    RUS
    hlt
}

factorial:
{
    push -1 + rbx
    push 0
    jbe exit_factorial

    push rbx
    push rcx
    mul
    pop rcx

    push -1 + rbx
    pop rbx

    call factorial

exit_factorial:
    push rcx
    push rbx
    mul
    pop rcx

    ret
}

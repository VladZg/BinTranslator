main:
{
    // in
    push 1
    pop rax
    // in
    push -5
    pop rbx
    // in
    push 6
    pop rcx

    call quadratic_solver

    push rdx
    out

    // print -1
    // print 2

    hlt
}

quadratic_solver:
{
    call calc_discreminant

    push rdx
    push 0
    jb neg_discreminant

    push rdx
    push 0
    je null_discreminant

pos_discreminant:
    push rdx
    push 1000
    mul
    fsqrt
    pop rdx
    // print 2
    jmp exit_quadratic_solver

neg_discreminant:
    // print 0
    jmp exit_quadratic_solver

null_discreminant:
    // print -1
    jmp exit_quadratic_solver

exit_quadratic_solver:
    ret
}

calc_discreminant:
{
    push rbx
    push rbx
    mul         // b^2
    push 4
    push rax
    mul
    push rcx
    mul         // 4ac
    sub         // b^2-4ac
    pop rdx

    ret
}

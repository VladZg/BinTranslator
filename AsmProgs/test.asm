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

    hlt
}

quadratic_solver:
{
    push rax
    push 0
    jne nlinear_solver
    call linear_solver
    jmp exit_quadratic_solver

nlinear_solver:
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

    // push 1000
    // push rbx
    // mul         // -b (*1000)
    // push rdx
    // add         // -b + sqrt(D)
    // push 2000
    // push rax
    // mul         // 2a
    // fdiv        // x1
    // pop r8
    // // out

    // push -1000
    // push rbx
    // mul         // -b (*1000)
    // push rdx
    // sub         // -b - sqrt(D)
    // push 2000
    // push rax
    // mul         // 2a
    // fdiv        // x2
    // pop r9
    // // out

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

linear_solver:
{
    push rbx
    push 0
    je inf_roots

    push -1
    push rcx
    mul
    push rbx
    div

inf_roots:
    print -1

exit_lin_solver:
    ret
}

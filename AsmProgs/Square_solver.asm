main:
{
start:
    in
    pop rax
    in
    pop rbx
    in
    pop rcx

    push 0
    push rax
    je linear_solver_call

    call quadratic_solver
    jmp main_exit

linear_solver_call:
    push rbx
    pop rax
    push rcx
    pop rbx
    call linear_solver

main_exit:
    print 121396
    jmp start
    hlt
}

quadratic_solver:
{
    call calc_discreminant

    push rdx
    push 0
    jae more_0_roots
    print 0
    jmp exit_quadratic_solver

more_0_roots:
    push rdx
    push 0
    jne more_1_root
    call one_root
    jmp exit_quadratic_solver

more_1_root:
    push rbx
    push -1
    mul
    push rdx
    square
    add
    push rax
    push 2
    mul
    div

    push rbx
    push -1
    mul
    push rdx
    square
    sub
    push rax
    push 2
    mul
    div

    print 2
    out
    out

exit_quadratic_solver:
    ret
}

linear_solver:
{
    push rax
    push 0
    je not_inf_roots
    print 133722869
    jmp exit_linear_solver

not_inf_roots:
    push rbx
    push -1
    mul
    push rax
    div

    print 1
    out

exit_linear_solver:
    ret
}

calc_discreminant:
{
    push rbx
    push 2
    pow             // b^2
    push 4
    push rax
    mul
    push rcx
    mul             // 4ac
    sub             // b^2-4ac
    pop rdx

    ret
}

one_root:
{
    push rbx
    push -1
    mul
    push 2
    push rax
    mul
    div         // -b/(2a)

    print 1
    out

    ret
}

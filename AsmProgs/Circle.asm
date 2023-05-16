main:
{
    in                  // radius
    pop rdx
    call draw_circle
    hlt
}

clean_regs:
{
    push 0
    push 0
    push 0
    pop rax
    pop rbx
    pop rcx

    ret
}

fill_cell:
{
    pop rax
    push 0
    pop [rax]

    ret
}


fill_circle:
{
    push 1 + rbx
    pop rbx

    push 87266
    push rcx
    add
    pop rcx

    push rcx
    cos
    push rdx
    mul
    push 2
    mul
    push 41
    add                 // x coordinate

    push rcx
    sin
    push rdx
    mul
    push 11
    add                 // y coordinate
    pop rfx

    push -1 + rfx
    push 81
    mul
    add

    call fill_cell

    push 1 + rbx
    push 720
    jbe fill_circle

    ret
}

draw_circle:
{
    call clean_regs

    CLEAR_RAM
    call fill_circle
    CLEAR_CONSOLE
    RAM
    DELAY 100

    ret
}

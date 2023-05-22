push 1
pop rax

push [1+2]
out

push [rax]
out

push [1+rax]
out

push [7]
out

pop [2]
pop [1+rax]

hlt

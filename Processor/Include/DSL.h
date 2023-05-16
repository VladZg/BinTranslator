#define S_POP                           \
    StackPop(&STACK)

#define S_PUSH( val )                   \
    StackPush(&STACK, (Elem_t)(val))

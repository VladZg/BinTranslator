#ifndef BIN_DSL_H
#define BIN_DSL_H

#ifdef DEF_BIN_DSL

    #define RET 0xC3
    #define NOP 0x90

    #define PUSH_RAX 0x50
    #define PUSH_RCX 0x51
    #define PUSH_RDX 0x52
    #define PUSH_RBX 0x53
    #define PUSH_RSP 0x54
    #define PUSH_RBP 0x55
    #define PUSH_RSI 0x56
    #define PUSH_RDI 0x57
    #define PUSH_R8  0x41, 0x50
    #define PUSH_R9  0x41, 0x51
    #define PUSH_R10 0x41, 0x52
    #define PUSH_R11 0x41, 0x53
    #define PUSH_R12 0x41, 0x54
    #define PUSH_R13 0x41, 0x55
    #define PUSH_R14 0x41, 0x56

    #define POP_RAX  0x58
    #define POP_RCX  0x59
    #define POP_RDX  0x5A
    #define POP_RBX  0x5B
    #define POP_RSP  0x5C
    #define POP_RBP  0x5D
    #define POP_RSI  0x5E
    #define POP_RDI  0x5F
    #define POP_R8   0x41, 0x58
    #define POP_R9   0x41, 0x59
    #define POP_R10  0x41, 0x5A
    #define POP_R11  0x41, 0x5B
    #define POP_R12  0x41, 0x5C
    #define POP_R13  0x41, 0x5D
    #define POP_R14  0x41, 0x5E

    #define SAVE_REGS   \
            PUSH_R8,    \
            PUSH_R9,    \
            PUSH_R10,   \
            PUSH_R11

    #define LOAD_REGS   \
            POP_R11,    \
            POP_R10,    \
            POP_R9,     \
            POP_R8

    #define PUSHA       \
            PUSH_RAX,   \
            PUSH_RCX,   \
            PUSH_RDX,   \
            PUSH_RBX,   \
            PUSH_RSP,   \
            PUSH_RBP,   \
            PUSH_RSI,   \
            PUSH_RDI,   \
            PUSH_R8 ,   \
            PUSH_R9 ,   \
            PUSH_R10,   \
            PUSH_R11,   \
            PUSH_R12,   \
            PUSH_R13,   \
            PUSH_R14

    #define POPA        \
            POP_R14,    \
            POP_R13,    \
            POP_R12,    \
            POP_R11,    \
            POP_R10,    \
            POP_R9 ,    \
            POP_R8 ,    \
            POP_RDI,    \
            POP_RSI,    \
            POP_RBP,    \
            POP_RSP,    \
            POP_RBX,    \
            POP_RDX,    \
            POP_RCX,    \
            POP_RAX

    #define PUSH_X  0x68

    #define ADD_RSI_RDI 0x48, 0x01, 0xFE
    #define SUB_RSI_RDI 0x48, 0x29, 0xFE

    #define MOV_RAX
    #define MOV_R12  0x41, 0xBC

#else

    #undef RET
    #undef NOP

    #undef PUSH_RAX
    #undef PUSH_RCX
    #undef PUSH_RDX
    #undef PUSH_RBX
    #undef PUSH_RSP
    #undef PUSH_RBP
    #undef PUSH_RSI
    #undef PUSH_RDI
    #undef PUSH_R8
    #undef PUSH_R9
    #undef PUSH_R10
    #undef PUSH_R11
    #undef PUSH_R12
    #undef PUSH_R13
    #undef PUSH_R14

    #undef POP_RAX
    #undef POP_RCX
    #undef POP_RDX
    #undef POP_RBX
    #undef PUSH_RSP
    #undef PUSH_RBP
    #undef POP_RSI
    #undef POP_RDI
    #undef POP_R8
    #undef POP_R9
    #undef POP_R10
    #undef POP_R11
    #undef POP_R12
    #undef POP_R13
    #undef POP_R14

    #undef SAVE_REGS
    #undef LOAD_REGS
    #undef PUSHA
    #undef POPA

    #define ADD_RSI_RDI
    #define SUB_RSI_RDI

    #undef MOVE_RAX
    #undef MOV_R12

#endif

#endif

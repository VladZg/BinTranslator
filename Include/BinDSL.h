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
            POP_RBX,    \
            POP_RDX,    \
            POP_RCX,    \
            POP_RAX

    #define PUSH_X  0x68
    #define PUSH_0  0x6A, 0x00

    #define ADD_RSP_x   0x48, 0x83, 0xC4
    #define ADD_RSP_X   0x48, 0x81, 0xC4
    #define SUB_RSP_x   0x48, 0x83, 0xEC
    #define SUB_RSP_X   0x48, 0x81, 0xEC
    #define ADD_RSI_RDI 0x48, 0x01, 0xFE
    #define ADD_R13_R14 0x4D, 0x01, 0xF5
    #define SUB_RSI_RDI 0x48, 0x29, 0xFE
    #define SUB_R13_R14 0x4D, 0x29, 0xF5
    #define ADD_RAX_RBX 0x48, 0x01, 0xD8
    #define SUB_RAX_RBX 0x48, 0x29, 0xD8
    #define IMUL_RBX    0x48, 0xF7, 0xEB
    #define IMUL_R13    0x49, 0xF7, 0xED
    #define IMUL_R14    0x49, 0xF7, 0xEE
    #define IDIV_RBX    0x48, 0xF7, 0xFB
    #define IDIV_R14D   0x41, 0xF7, 0xFE
    #define IDIV_R14    0x49, 0xF7, 0xFE
    #define DIV_R14     0x49, 0xF7, 0xF6

    #define CDQE        0x48, 0x98

    #define MOV_RAX     0xB8
    #define MOV_R12     0x41, 0xBC
    #define MOV_R14     0x49, 0xBE
    #define MOV_EDI     0xDF
    #define MOV_RDI     0x48, 0xBF
    #define MOV_RDI_RAX 0x48, 0x89, 0xC7
    #define MOV_RDI_R14 0x4C, 0x89, 0xF7
    #define MOV_RBP_RSP 0x48, 0x89, 0xE5
    #define MOV_R14_RAX 0x49, 0x89, 0xC6
    #define MOV_RAX_R14 0x4C, 0x89, 0xF0
    #define MOV_R13_RAX 0x49, 0x89, 0xC5
    #define MOV_RAX_R13 0x4C, 0x89, 0xE8

    // actually jg, jge, jl, jle
    #define REL_JMP     0xE9
    #define REL_JA      0x0F, 0x8F
    #define REL_JAE     0x0F, 0x8D
    #define REL_JB      0x0F, 0x8C
    #define REL_JBE     0x0F, 0x8E
    #define REL_JE      0x0F, 0x84
    #define REL_JNE     0x0F, 0x85
    #define REL_JF
    #define JMP_RAX     0xFF, 0xE0
    #define REL_CALL    0xE8

    #define CALL_R14    0x41, 0xFF, 0xD6

    #define CMP_RAX_RBX         0x48, 0x39, 0xD8
    #define CMP_R13D_R14D       0x45, 0x39, 0xF5
    #define CMP_R13_R14         0x4D, 0x39, 0xF5

    #define FINIT               0x9B, 0xDB, 0xE3
    #define FABS                0xD9, 0xE1
    #define FDIV                0xDE, 0xF9
    #define FMUL                0xDE, 0xC9
    #define FSQRT               0xD9, 0xFA
    #define FILD_QWORD_RSP      0xDF, 0x2C, 0x24
    #define FILD_QWORD_RSP_x    0xDF, 0x6C, 0x24
    #define FISTP_QWORD_RSP     0xDF, 0x3C, 0x24

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

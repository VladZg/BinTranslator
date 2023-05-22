#include "../Config.h"
#include <stdlib.h>
#include <cstring>
#include <sys/mman.h>
#include <limits.h>
#include "../Include/Assert.h"
#include "../Include/Defines.h"
#include "../Include/IR.h"
#include "../Include/Translator.h"

#define BUF         (x86_buf->buf         )
#define BUF_ADDRESS ((ADDRESS)x86_buf->buf)
#define RAM_ADDRESS (x86_buf->RAM_addr    )
#define INDEX       (x86_buf->index       )
#define BUF_SIZE    (x86_buf->buf_size    )
#define PROG_SIZE   (x86_buf->prog_size   )

void PasteByteInBuf(x86Buf* x86_buf, BYTE byte)
{
    VERIFY_x86BUF(x86_buf)

    BUF[INDEX++] = byte;
    PROG_SIZE++;
}

void PasteNBytesInBuf(x86Buf* x86_buf, const BYTE* bytes, size_t n_bytes)
{
    VERIFY_x86BUF(x86_buf)
    ASSERT(bytes != nullptr);

    memcpy(BUF + INDEX, bytes, n_bytes);
    INDEX     += n_bytes;
    PROG_SIZE += n_bytes;
}

// void FillBuf(BYTE* buf, size_t buf_size, BYTE byte)
// {
//     memset(buf, buf_size, byte);
// }

int x86BufCtor(x86Buf* x86_buf, CtorDumpMode mode)
{
    ASSERT(x86_buf != nullptr);

    INDEX = 0;
    BUF_SIZE  = MAX_PROGRAMM_SIZE;
    PROG_SIZE = 0;
    BUF   = (BYTE*) aligned_alloc(PAGESIZE, BUF_SIZE);
    ASSERT(BUF != nullptr);

    int mprotect_res = mprotect(BUF, BUF_SIZE, PROT_READ | PROT_WRITE);
    ASSERT(mprotect_res == 0);

    memset(BUF, BUF_SIZE, NULL);

    if (mode == CTOR_DUMP_MODE) x86BufDump(x86_buf, 256);

    return 1;
}

int x86BufVerify(const x86Buf* x86_buf)
{
    if (PROG_SIZE > BUF_SIZE)
        FUNC_NAME(printf("Programm size is greater than buffer size!\n");)

    else if (INDEX > BUF_SIZE)
        FUNC_NAME(printf("Index is out of buffer!\n");)

    else if (BUF == nullptr)
        FUNC_NAME(printf("Buffer pointer is nullptr!\n");)

    else return 1;

    return 0;
}

int x86BufDtor(x86Buf* x86_buf)
{
    if (x86_buf == nullptr) return 1;

    BUF_SIZE  = 0;
    PROG_SIZE = 0;
    INDEX     = 0;
    *BUF      = NULL;
    free(BUF);
    BUF       = nullptr;

    return 1;
}

#define DEF_BIN_DSL
#include "../Include/BinDSL.h"

int x86BufDump(const x86Buf* x86_buf, size_t n_bytes)
{
    VERIFY_x86BUF(x86_buf)

    FUNC_NAME(
    printf("    buffer size              = " KGRN "%u" KNRM "\n", BUF_SIZE     );
    printf("    programm size            = " KRED "%u" KNRM "\n", PROG_SIZE    );
    printf("    index                    = " KYEL "%u" KNRM "\n", INDEX        );
    printf("    buffer address           = %016p             \n", BUF_ADDRESS  );
    printf("    RAM address              = %016p             \n", RAM_ADDRESS  );
    printf("    buf (first %6u bytes) = {                    \n", n_bytes      );
    BinCodeDump(BUF, n_bytes, "                                ", LIGHT_DUMP_MODE, INDEX, NOP); //x86_buf.size
    printf("                               }                 \n"               );)

    return 1;
}

#ifndef N_TEXT_TRANSLATION_MODE

    FILE* text_translation_file = fopen("./Debug/text_translation.txt", "w");

    #define TextTranslate(...)                              \
        fprintf(text_translation_file, __VA_ARGS__);

    void exit_function() {fclose(text_translation_file);}
    int exit_status = atexit(exit_function);

#else
    #define TextTranslate(...)
#endif

#define PasteByte(  byte ) PasteByteInBuf(x86_buf, byte);

#define PasteBytes( ...  )                                  \
{                                                           \
    BYTE cmd[] = {__VA_ARGS__};                             \
    PasteNBytesInBuf(x86_buf, cmd, sizeof(cmd));            \
}
#define PasteCmd(   cmd  ) PasteNBytesInBuf(x86_buf, cmd, sizeof(cmd));

#define PasteInt(   num  )                                  \
{                                                           \
    int int_num[] = {num};                                  \
    memcpy(BUF + INDEX, int_num, sizeof(int));              \
    INDEX+=sizeof(int); PROG_SIZE+=sizeof(int);             \
}

#define PasteLongInt(   num  )                              \
{                                                           \
    long int long_int_num[] = {num};                        \
    memcpy(BUF + INDEX, long_int_num, sizeof(long int));    \
    INDEX+=sizeof(long int); PROG_SIZE+=sizeof(long int);   \
}

#define PasteAddress(   addr  )                             \
{                                                           \
    ADDRESS addr_arr[] = {(ADDRESS) addr};                  \
    memcpy(BUF + INDEX, addr_arr, sizeof(ADDRESS));         \
    INDEX+=sizeof(ADDRESS); PROG_SIZE+=sizeof(ADDRESS);     \
}
    // printf("\n\naddress: %016X\n\n", *addr_arr);            \


#ifdef NOPPING
    #define PasteNOP PasteByte(NOP)
#else
    #define PasteNOP
#endif

static void TranslatePushCmd(x86Buf* x86_buf, const Command* cmd)
{
    ASSERT(cmd != nullptr);
    ASSERT(cmd->cmd_code == CMD_PUSH);
    VERIFY_x86BUF(x86_buf);

    for (size_t arg_j = 0; arg_j < cmd->n_args; arg_j++)
    {
        if (cmd->args[arg_j].type == ARG_NUM_TYPE)
        {
            PasteBytes(MOV_R14)                                  // mov r14, <num>
            PasteLongInt(cmd->args[arg_j].value)
            PasteBytes(PUSH_R14)                                 // push r14

            TextTranslate("    mov   r14, %016X \n"
                          "    push  r14        \n", cmd->args[arg_j].value)
        }

        else if (cmd->args[arg_j].type == ARG_REG_TYPE)
        {
            #define DEF_REG(name, num)                                          \
                case REG_##name:                                                \
                {                                                               \
                    PasteBytes(PUSH_##name);                                    \
                    TextTranslate("    push %s\n", #name)                       \
                    break;                                                      \
                }                                                                   // push <reg>

            switch (cmd->args[arg_j].value)
            {
                #include "../Processor/Include/Reg.h"
            }

            #undef DEF_REG
        }
    }

    if (cmd->n_args == 2)
    {
        PasteBytes(POP_R13    ,     // pop r13
                   POP_R14    ,     // pop r14
                   ADD_R13_R14,     // add r13, r14
                   PUSH_R13   )     // push r13

        TextTranslate("    pop  r13     \n"
                      "    pop  r14     \n"
                      "    add  r13, r14\n"
                      "    push r13     \n")
    }

    if (cmd->loc == ARG_MEM_LOC)
    {

//         PasteBytes(     POP_R13,
//                         MOV_R14                )
//         PasteAddress(   RAM_ADDRESS            )
//         PasteBytes(     // 0x4F, 0x8D, 0x34, 0xEE,
//                         // 0x4D, 0x8B, 0x2E,
//                         0x4F, 0x8B, 0x2C, 0xEE,
//                         PUSH_R13               )
//
//         TextTranslate("    pop  r13                         \n"
//                       "    mov  r14, %16X                   \n"
//                     //   "    lea  r14, [r14 + r13 * 8]        \n"
//                       "    mov  r13, [r14 + r13 * 8]        \n"
//                       "    push r13                         \n", RAM_ADDRESS)
    }
}

static void TranslatePopCmd(x86Buf* x86_buf, const Command* cmd)
{
    ASSERT(cmd != nullptr);
    ASSERT(cmd->cmd_code == CMD_POP);
    VERIFY_x86BUF(x86_buf);

    if (cmd->loc == ARG_REG_LOC)
    {
        #define DEF_REG(name, num)                                          \
            case REG_##name:                                                \
            {                                                               \
                PasteBytes(POP_##name);                                     \
                TextTranslate("    pop  %s\n", #name)                       \
                break;                                                      \
            }                                                                   // pop <reg>

        switch (cmd->args[0].value)
        {
            #include "../Processor/Include/Reg.h"
        }

        #undef DEF_REG
    }
}

static void TranslateAddCmd(x86Buf* x86_buf, const Command* cmd)
{
    ASSERT(cmd != nullptr);
    ASSERT(cmd->cmd_code == CMD_ADD);
    VERIFY_x86BUF(x86_buf);

    PasteBytes(POP_R13    ,     // pop  rax
               POP_R14    ,     // pop  rbx
               ADD_R13_R14,     // add  rax, rbx
               PUSH_R13   )     // push rax

    TextTranslate("    pop  r13     \n"
                  "    pop  r14     \n"
                  "    add  r13, r14\n"
                  "    push r13    \n")
}

static void TranslateSubCmd(x86Buf* x86_buf, const Command* cmd)
{
    ASSERT(cmd != nullptr);
    ASSERT(cmd->cmd_code == CMD_SUB);
    VERIFY_x86BUF(x86_buf);

    PasteBytes(POP_R14    ,     // pop  rbx
               POP_R13    ,     // pop  rax
               SUB_R13_R14,     // sub  rax, rbx
               PUSH_R13   )     // push rax

    TextTranslate("    pop  r14     \n"
                  "    pop  r13     \n"
                  "    sub  r13, r14\n"
                  "    push r13     \n")
}

static void TranslateMulCmd(x86Buf* x86_buf, const Command* cmd)
{
    ASSERT(cmd != nullptr);
    VERIFY_x86BUF(x86_buf);

    if (cmd->cmd_code == CMD_MUL)
    {
        TextTranslate("MUL\n")

        PasteBytes(POP_R13    ,  // pop  r13
                   POP_R14    ,  // pop  r14
                   PUSH_RAX   ,  // push rax
                   MOV_RAX_R13,  // mov  rax, r13
                   IMUL_R14   ,  // imul r14
                   MOV_R13_RAX,  // mov  r13, rax
                   POP_RAX    ,  // pop  rax
                   PUSH_R13    ) // push r13

        TextTranslate("    pop  r13     \n"
                      "    pop  r14     \n"
                      "    push rax     \n"
                      "    mov  rax, r13\n"
                      "    imul r14     \n"
                      "    mov  r13, rax\n"
                      "    pop  rax     \n"
                      "    push r13     \n")
    }

    else if (cmd->cmd_code == CMD_FMUL)
    {
        TextTranslate("FMUL\n")

        PasteByte (PUSH_X                   )
        PasteInt  (1000                     )    // push qword 1000
        PasteBytes(FILD_QWORD_RSP_x , 0x10,      // fild qword [rsp+16]
                   FILD_QWORD_RSP_x , 0x08,      // fild qword [rsp+8]
                   FILD_QWORD_RSP   ,            // fild qword [rsp]
                   FDIV             ,       	 // fdiv
                   FMUL             ,       	 // fmul
                   ADD_RSP_x        , 0x10,      // add rsp, 16
                   FISTP_QWORD_RSP          )    // fistp [rsp]

        TextTranslate("    push  1000          \n"
                      "    fild  qword [rsp+16]\n"
                      "    fild  qword [rsp+8] \n"
                      "    fild  qword [rsp]   \n"
                      "    fdiv                \n"
                      "    fmul                \n"
                      "    add   rsp, 16       \n"
                      "    fistp qword [rsp]   \n")


    }
}

static void TranslateDivCmd(x86Buf* x86_buf, const Command* cmd)
{
    ASSERT(cmd != nullptr);
    VERIFY_x86BUF(x86_buf);

    if(cmd->cmd_code == CMD_DIV)
    {
        TextTranslate("DIV\n")

        PasteBytes(POP_R14    ,  // pop  r14
                   POP_R13    ,  // pop  r13
                   PUSH_RAX   ,  // push rax
                   PUSH_RDX   ,  // push rdx
                //    MOV_RAX_R14,  // mov rax, r14
                //    CDQE       ,  // cqde
                //    MOV_R14_RAX,  // mov r14, rax
                   MOV_RAX_R13,  // mov  rax, r13
                //    CDQE       ,  // cdqe
                   IDIV_R14   ,  // idiv r14
                   MOV_R13_RAX,  // mov  r13, rax
                   POP_RDX    ,  // pop rdx
                   POP_RAX    ,  // pop  rax
                   PUSH_R13    ) // push r13

        TextTranslate("    pop  r14      \n"
                      "    pop  r13      \n"
                      "    push rax      \n"
                      "    push rdi      \n"
                    //   "    mov  rax, r14 \n"
                    //   "    cdqe          \n"
                    //   "    mov  r14, rax \n"
                      "    mov  rax, r13 \n"
                    //   "    cdqe          \n"
                      "    idiv r14      \n"
                      "    mov  r13, rax \n"
                      "    pop  rdi      \n"
                      "    pop  rax      \n"
                      "    push r13      \n")
    }

    else if (cmd->cmd_code == CMD_FDIV)
    {
        TextTranslate("FDIV\n")

        PasteByte (PUSH_X                 )
        PasteInt  (1000                   )        // push qword 1000
        PasteBytes(FILD_QWORD_RSP  ,               // fild qword [rsp]
                   FILD_QWORD_RSP_x, 0x10,         // fild qword [rsp+16]
                   FILD_QWORD_RSP_x, 0x08,         // fild qword [rsp+8]
                   FDIV            ,       	       // fdiv
                   FMUL            ,       	       // fmul
                   ADD_RSP_x       , 0x10,         // add rsp, 16
                   FISTP_QWORD_RSP       )         // fistp qword [rsp]


        TextTranslate("    push 1000          \n"
                      "    fild qword [rsp]   \n"
                      "    fild qword [rsp+8] \n"
                      "    fild qword [rsp+16]\n"
                      "    fdiv               \n"
                      "    fmul               \n"
                      "    add rsp, 16        \n"
                      "    fistp qword [rsp]  \n")
    }
}

static void TranslateGotoCmd(x86Buf* x86_buf, const Command* cmd)
{
    ASSERT(cmd != nullptr);
    VERIFY_x86BUF(x86_buf);

    #define DEF_CMD(...)
    #define DEF_DUMP(...)
    #define DEF_JMP(name, num, condition, ...)                          \
        case CMD_##name:                                                \
            TextTranslate("; %s\n", #name) break;

    switch (cmd->cmd_code)
    {
        #include "../Processor/Include/Cmd.h"
    }

    #undef DEF_CMD
    #undef DEF_DUMP
    #undef DEF_JMP

    switch (cmd->cmd_code)
    {
        case CMD_JA  :
        case CMD_JAE :
        case CMD_JB  :
        case CMD_JBE :
        case CMD_JE  :
        case CMD_JNE :
        {
            PasteBytes(POP_R14      ,
                       POP_R13      ,
                       CMP_R13_R14   )

            TextTranslate("    pop  r14       \n"
                          "    pop  r13       \n"
                          "    cmp  r13, r14  \n")
            break;
        }
    }

    #define DEF_CMD(...)
    #define DEF_DUMP(...)
    #define DEF_JMP(name, num, condition, ...)                          \
        case CMD_##name:                                                \
        {                                                               \
            PasteBytes(REL_##name);                                     \
            TextTranslate("    %s %d\n", #name, 0)                      \
            break;                                                      \
        }

    switch (cmd->cmd_code)
    {
        #include "../Processor/Include/Cmd.h"
    }

    #undef DEF_CMD
    #undef DEF_DUMP
    #undef DEF_JMP

    PasteBytes(0x00, 0x00, 0x00, 0x00) // пока не известен относительный адрес перехода

    // PasteInt((uint32_t) command_i.args[0].value - (command_i.pc + 1 + sizeof(int)));
    // PasteInt((uint32_t) offsets[command_i.args[0].value] - offsets[command_i.pc.value])
}

static void TranslateRetCmd(x86Buf* x86_buf, const Command* cmd)
{
    ASSERT(cmd != nullptr);
    ASSERT(cmd->cmd_code == CMD_HLT || cmd->cmd_code == CMD_RET);
    VERIFY_x86BUF(x86_buf);

    PasteByte(RET)
    TextTranslate("    ret\n")
}

int TranslateCmd_x86(x86Buf* x86_buf, const Command* cmd)
{
    VERIFY_x86BUF(x86_buf)
    ASSERT(cmd != nullptr);

    switch (cmd->cmd_code)
    {
        case CMD_PUSH:
        {
            TextTranslate("PUSH\n")
            TranslatePushCmd(x86_buf, cmd);
            break;
        }

        case CMD_POP:
        {
            TextTranslate("POP\n")
            TranslatePopCmd(x86_buf, cmd);
            break;
        }

        case CMD_ADD:
        {
            TextTranslate("ADD\n")
            TranslateAddCmd(x86_buf, cmd);
            break;
        }

        case CMD_SUB:
        {
            TextTranslate("SUB\n")
            TranslateSubCmd(x86_buf, cmd);
            break;
        }

        case CMD_MUL:
        {
            TextTranslate("MUL\n")
            TranslateMulCmd(x86_buf, cmd);
            break;
        }

        case CMD_DIV:
        {
            TextTranslate("DIV\n")
            TranslateDivCmd(x86_buf, cmd);
            break;
        }

        case CMD_CALL:
        case CMD_JMP :
        case CMD_JA  :
        case CMD_JAE :
        case CMD_JB  :
        case CMD_JBE :
        case CMD_JE  :
        case CMD_JNE :
        {
            TranslateGotoCmd(x86_buf, cmd);
            break;
        }

        case CMD_RET:
        {
            TextTranslate("RET\n")
            TranslateRetCmd(x86_buf, cmd);
            break;
        }

        case CMD_HLT:
        {
            TextTranslate("HLT\n")
            PasteBytes(POPA)
            TranslateRetCmd(x86_buf, cmd);
            break;
        }
    }
}

long int InCmd()
{
    long int num = 0;
    printf("Type a number: ");
    scanf("%ld", &num);
    return num;
}

void OutCmd(long int num)
{
    printf("OUT: %016X\n", num);
    // printf("OUT: %ld\n", num);
}

void PrintCmd(long int num)
{
    printf("ARGUMENT: %ld\n", num);
}

void InitRAM(const IR* ir, x86Buf* x86_buf)
{
    TextTranslate("; Init RAM\n")

    PasteBytes( SUB_RSP_X   )
    PasteInt  ( RAM_SIZE    )   // sub rsp, <RAM_size>
    PasteBytes( MOV_RBP_RSP )   // mov rbp, rsp
                // REL_JMP      )
    // PasteInt(   RAM_SIZE     )

    // x86_buf->RAM_addr = (ADDRESS)(BUF + 1 + sizeof(int));

    // INDEX+=RAM_SIZE; PROG_SIZE+=RAM_SIZE;
    PasteNOP

    TextTranslate(  "    sub rsp, <RAM_SIZE>    \n"
                    "    mov rbp, rsp           \n\n")
                    // "    jmp %u         \n\n", RAM_SIZE)
}

int IRTranslate_x86(const IR* ir, x86Buf* x86_buf)
{
    VERIFY_IR(ir)
    VERIFY_x86BUF(x86_buf)

    #ifndef N_TEXT_TRANSLATION_MODE
        fseek(text_translation_file, 0, SEEK_SET);
    #endif

    TextTranslate("; Assembly logging of binary translation\n\n")

    BYTE start[] = {PUSHA       ,
                    MOV_RBP_RSP };
                    // FINIT        };
    PasteCmd(start)
    PasteNOP

    TextTranslate("start:       \n"
                  "    pusha    \n"
                  "    finit    \n\n")

    // InitRAM(ir, x86_buf);

    size_t offsets[ir->info.code_size] = {};

    for (size_t index = 0; index < ir->n_cmds; index++)
    {
        Command command_i = ir->commands[index];

        offsets[command_i.pc] = INDEX;
        TextTranslate("; [%u] ", index)

        // TranslateCmd_x86(x86_buf, &command_i);

        switch (command_i.cmd_code)
        {
            case CMD_PUSH:
            {
                TextTranslate("PUSH\n")
                TranslatePushCmd(x86_buf, &command_i);

                break;
            }

            case CMD_POP:
            {
                TextTranslate("POP\n")
                TranslatePopCmd(x86_buf, &command_i);

                break;
            }

            case CMD_FABS:
            {
                TextTranslate("FABS\n")
                PasteBytes( FILD_QWORD_RSP,	    // fild qword [rsp]
                            FABS,               // fabs
                            FISTP_QWORD_RSP)    // fistp qword [rsp]

                TextTranslate("    fild qword [rsp] \n"
                              "    fabs             \n"
                              "    fistp qword [rsp]\n")

                break;
            }

            case CMD_ADD:
            {
                TextTranslate("ADD\n")
                TranslateAddCmd(x86_buf, &command_i);

                break;
            }

            case CMD_SUB:
            {
                TextTranslate("SUB\n")
                TranslateSubCmd(x86_buf, &command_i);

                break;
            }

            case CMD_MUL:
            case CMD_FMUL:
            {
                TranslateMulCmd(x86_buf, &command_i);

                break;
            }

            case CMD_DIV:
            case CMD_FDIV:
            {
                TranslateDivCmd(x86_buf, &command_i);

                break;
            }

            case CMD_CALL:
            case CMD_JMP :
            case CMD_JA  :
            case CMD_JAE :
            case CMD_JB  :
            case CMD_JBE :
            case CMD_JE  :
            case CMD_JNE :
            {
                TranslateGotoCmd(x86_buf, &command_i);
                break;
            }

            case CMD_FSQRT:
            {
                PasteByte ( PUSH_X                 )
                PasteInt  ( 1000                   )        // push qword 1000
                PasteBytes( FILD_QWORD_RSP  ,               // fild qword [rsp]
                            FILD_QWORD_RSP_x, 0x08,         // fild qword [rsp+8]
                            FILD_QWORD_RSP  ,               // fild qword [rsp]
                            ADD_RSP_x       , 0x08,         // add rsp, 8
                            FDIV ,                          // fdiv
                            FSQRT,                          // fsqrt
                            FMUL ,                          // fmul
                            FISTP_QWORD_RSP        )        // fistp qword [rsp]

                break;
            }

            case CMD_IN:
            {
                TextTranslate("IN\n")

                PasteBytes( PUSH_RDI, PUSH_RAX, PUSH_RSI,
                            MOV_R14                         )
                PasteAddress((ADDRESS)InCmd                 )
                PasteBytes  (CALL_R14   ,
                             MOV_R14_RAX,
                             POP_RSI, POP_RAX, POP_RDI,
                             PUSH_R14                       )

                // PasteBytes(PUSHA, POPA, PUSH_RAX)

                // PasteBytes(PUSH_RAX)

                // printf("%p\n", InCmd);

                TextTranslate("    push rdi           \n"
                              "    push rax           \n"
                              "    push rsi           \n"
                              "    mov  r14, %p       \n"
                              "    call r14           \n"
                              "    mov  r14, rax      \n"
                              "    push rsi           \n"
                              "    push rax           \n"
                              "    push rdi           \n"
                              "    push r14           \n", InCmd)

                break;
            }

            case CMD_OUT:
            {
                TextTranslate("OUT\n")

                PasteBytes  (POP_R14, PUSHA, MOV_RDI_R14, MOV_R14   )
                PasteAddress((ADDRESS)OutCmd                        )
                PasteBytes  (CALL_R14, POPA                         )

                // PasteBytes(POP_RAX, PUSHA, POPA)

                // printf("%p\n", OutCmd);

                TextTranslate("    pusha              \n"
                              "    pop  r14           \n"
                              "    mov  rdi, r14      \n"
                              "    mov  r14, %p       \n"
                              "    call r14           \n"
                              "    popa               \n", OutCmd)

                break;
            }

            case CMD_PRINT:
            {
                TextTranslate("PRINT\n")

                PasteBytes  (PUSHA,                                   // PUSH_RDI, PUSH_RAX, PUSH_RSI,
                             MOV_RDI                        )
                PasteLongInt(command_i.args[0].value        )
                PasteBytes  (MOV_R14                        )
                PasteAddress((ADDRESS)PrintCmd              )
                PasteBytes  (CALL_R14,
                             POPA                           )           //POP_RSI, POP_RAX, POP_RDI      )

                // PasteBytes(POP_RAX, PUSHA, POPA)

                // printf("%p\n", PrintCmd);

                TextTranslate("    pusha              \n"
                              "    pop  r14           \n"
                              "    mov  rdi, %ld      \n"
                              "    mov  r14, %p       \n"
                              "    call r14           \n"
                              "    popa               \n"
                              , command_i.args[0].value, PrintCmd)

                break;
            }

            case CMD_RET:
            {
                TextTranslate("HLT\n")
                TranslateRetCmd(x86_buf, &command_i);

                break;
            }

            case CMD_HLT:
            {
                TextTranslate("HLT\n")
                BYTE end[] = {POPA};
                PasteCmd(end)
                TranslateRetCmd(x86_buf, &command_i);

                break;
            }
        }

        TextTranslate("\n")
        PasteNOP
    }

    // for(size_t i = 0; i < ir->info.code_size; i++) printf("%u ", offsets[i]);  printf("\n\n");

    // PatchAddresses(ir, x86_buf);

    for (size_t index = 0; index < ir->n_cmds; index++)
    {
        Command command_i = ir->commands[index];

        switch(command_i.cmd_code)
        {
            case CMD_CALL:
            case CMD_JMP :
            case CMD_JA  :
            case CMD_JAE :
            case CMD_JB  :
            case CMD_JBE :
            case CMD_JE  :
            case CMD_JNE :
            {
                size_t jmp_cmd_len = 1;

                if (command_i.cmd_code != CMD_CALL && command_i.cmd_code != CMD_JMP)
                    jmp_cmd_len = 9;    // jmp с параметрами

                uint32_t cmd_addr = offsets[command_i.pc];
                uint32_t jmp_addr = offsets[command_i.args[0].value] - (cmd_addr + jmp_cmd_len + sizeof(int));
                memcpy(BUF + cmd_addr + jmp_cmd_len, &jmp_addr, sizeof(int));
            }
        }
    }

    // x86BufDump(*x86_buf);

    return 1;
}

int PatchAddresses(const IR* ir, x86Buf* x86_buf, size_t* offsets)
{
    ASSERT(offsets != nullptr)
    VERIFY_IR(ir)
    VERIFY_x86BUF(x86_buf)

    for (size_t index = 0; index < ir->n_cmds; index++)
    {
        Command command_i = ir->commands[index];

        switch(command_i.cmd_code)
        {
            case CMD_CALL:
            case CMD_JMP :
            case CMD_JA  :
            case CMD_JAE :
            case CMD_JB  :
            case CMD_JBE :
            case CMD_JE  :
            case CMD_JNE :
            {
                size_t jmp_cmd_len = 1;

                if (command_i.cmd_code != CMD_CALL && command_i.cmd_code != CMD_JMP)
                    jmp_cmd_len = 7;    // jmp с параметрами

                uint32_t cmd_addr = offsets[command_i.pc];
                uint32_t jmp_addr = offsets[command_i.args[0].value] - (cmd_addr + jmp_cmd_len + sizeof(int));
                memcpy(BUF + cmd_addr + jmp_cmd_len, &jmp_addr, sizeof(int));
            }
        }
    }
}

#undef DEF_BIN_DSL
#include "../Include/BinDSL.h"

#undef  BUF
#undef  INDEX
#define BUF_SIZE
#define PROG_SIZE
#undef  PASTE

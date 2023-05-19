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
    printf("    buf_size                 = " KGRN "%u" KNRM "\n", BUF_SIZE     );
    printf("    prog_size                = " KRED "%u" KNRM "\n", PROG_SIZE    );
    printf("    index                    = " KYEL "%u" KNRM "\n", INDEX        );
    printf("    address                  = %016p             \n", BUF_ADDRESS);
    printf("    buf (first %6u bytes) = {                \n"    , n_bytes      );
    BinCodeDump(BUF, n_bytes, "                                ", LIGHT_DUMP_MODE, INDEX, NOP); //x86_buf.size
    printf("                               } \n"                               );)

    return 1;
}

#ifndef N_TEXT_TRANSLATION_MODE
    extern FILE* text_translation_file;

    #define TextTranslate(...)                              \
        fprintf(text_translation_file, __VA_ARGS__);
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
            PasteByte(PUSH_X)                                   // push <num>
            PasteInt(cmd->args[arg_j].value)

            TextTranslate("    push %d \n", cmd->args[arg_j].value)
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

                default: PasteByte(RET)
            }

            #undef DEF_REG
        }
    }

    if (cmd->n_args == 2)
    {
        PasteBytes(POP_RSI    ,     // pop rsi
                   POP_RDI    ,     // pop rdi
                   ADD_RSI_RDI,     // add rsi, rdi
                   PUSH_RSI   )     // push rsi

        TextTranslate("    pop rsi     \n"
                      "    pop rdi     \n"
                      "    add rsi, rdi\n"
                      "    push rsi    \n")
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

            default: PasteByte(RET)
        }

        #undef DEF_REG
    }
}

static void TranslateAddCmd(x86Buf* x86_buf, const Command* cmd)
{
    ASSERT(cmd != nullptr);
    ASSERT(cmd->cmd_code == CMD_ADD);
    VERIFY_x86BUF(x86_buf);

    PasteBytes(POP_RSI    ,     // pop rsi
               POP_RDI    ,     // pop rdi
               ADD_RSI_RDI,     // add rsi, rdi
               PUSH_RSI   )     // push rsi

    TextTranslate("    pop rsi     \n"
                  "    pop rdi     \n"
                  "    add rsi, rdi\n"
                  "    push rsi    \n")
}

static void TranslateSubCmd(x86Buf* x86_buf, const Command* cmd)
{
    ASSERT(cmd != nullptr);
    ASSERT(cmd->cmd_code == CMD_SUB);
    VERIFY_x86BUF(x86_buf);

    PasteBytes(POP_RDI    ,     // pop rdi
               POP_RSI    ,     // pop rsi
               SUB_RSI_RDI,     // sub rsi, rdi
               PUSH_RSI   )     // push rsi

    TextTranslate("    pop rsi     \n"
                  "    pop rdi     \n"
                  "    sub rsi, rdi\n"
                  "    push rsi    \n")
}

static void TranslateHltCmd(x86Buf* x86_buf, const Command* cmd)
{
    ASSERT(cmd != nullptr);
    ASSERT(cmd->cmd_code == CMD_HLT);
    VERIFY_x86BUF(x86_buf);

    BYTE end[] = {RET};
    PasteCmd(end)

    TextTranslate("    ret\n")
}

int IRTranslate_x86(const IR* ir, x86Buf* x86_buf)
{
    VERIFY_IR(ir)
    VERIFY_x86BUF(x86_buf)

    BYTE init[] = {}; // PUSHA
    PasteCmd(init)

    for (size_t index = 0; index < ir->n_cmds; index++)
    {
        Command command_i = ir->commands[index];

        TextTranslate("; [%u] ", index)

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

            case CMD_HLT:
            {
                TextTranslate("HLT\n")
                TranslateHltCmd(x86_buf, &command_i);

                break;
            }
        }

        TextTranslate("\n")
        PasteNOP
    }

    // x86BufDump(*x86_buf);

    return 1;
}

#undef DEF_BIN_DSL
#include "../Include/BinDSL.h"

#undef  BUF
#undef  INDEX
#define BUF_SIZE
#define PROG_SIZE
#undef  PASTE

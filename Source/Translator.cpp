#include "../Config.h"
#include <stdlib.h>
#include <cstring>
#include <sys/mman.h>
#include <limits.h>
#include "../Include/Assert.h"
#include "../Include/Defines.h"
#include "../Include/IR.h"
#include "../Include/Translator.h"

void PasteInBuf(BYTE* buf, size_t* buf_index, BYTE byte)
{
    buf[(*buf_index)++] = byte;
}

void FillBuf(BYTE* buf, size_t buf_size, BYTE byte)
{
    memset(buf, buf_size, byte);
}

void EndBuf(BYTE* buf, size_t* buf_index)
{
    PasteInBuf(buf, buf_index, 0xC3);   // retn
}

void print_buf(const BYTE* buf, int buf_size)
{
    ASSERT(buf != nullptr);
    int i = 0;
    // printf("%d\n", buf_size);
    while(i < buf_size) printf("%02X ", buf[i++]);
    printf("\n");
}

#define BUF     (x86_buf->buf  )
#define INDEX   (x86_buf->index)
#define SIZE    (x86_buf->size )
#define PASTE(byte) PasteInBuf(BUF, &INDEX, byte)

int x86BufCtor(x86Buf* x86_buf, CtorDumpMode mode)
{
    ASSERT(x86_buf != nullptr);

    INDEX = 0;
    SIZE  = MAX_PROGRAMM_SIZE;
    BUF   = (BYTE*) aligned_alloc(PAGESIZE, SIZE);
    ASSERT(BUF != nullptr);

    int mprotect_res = mprotect(BUF, SIZE, PROT_READ | PROT_WRITE);
    ASSERT(mprotect_res == 0);

    if (mode == CTOR_DUMP_MODE) x86BufDump(*x86_buf);

    return 1;
}

int x86BufVerify(x86Buf x86_buf)
{


    return 1;
}

int x86BufDtor(x86Buf* x86_buf)
{
    if (x86_buf == nullptr) return 1;

    SIZE = 0;
    INDEX = 0;
    *BUF = NULL;
    free(BUF);
    BUF = nullptr;

    return 1;
}

int x86BufDump(x86Buf x86_buf)
{
    VERIFY_x86BUF(x86_buf)

    FUNC_NAME(
    printf("    size  = " KYEL "%u" KNRM "\n", x86_buf.size );
    printf("    index = " KYEL "%u" KNRM "\n", x86_buf.index);
    printf("    buf   = { \n"                               );
    BinCodeDump(x86_buf.buf, 256, "             ");)
    printf("            } \n"                               );

    return 1;
}

int IRTranslate_x86(const IR* ir, x86Buf* x86_buf)
{
    ASSERT(ir != nullptr);
    ASSERT(x86_buf != nullptr);
    VERIFY_IR(*ir)
    VERIFY_x86BUF(*x86_buf)

    FillBuf(BUF, SIZE, 0x00);

    // PASTE(0x60);    // pusha
    // PASTE(0x61);    // popa
    PASTE(0xC3);    // retn
    // print_buf(buf, buf_size);

    // x86BufDump(*x86_buf);

    return 1;
}

#undef BUF
#undef INDEX
#undef SIZE
#undef PASTE

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "../Config.h"
#include "./Defines.h"
#include "./IR.h"

#define MAX_PROGRAMM_SIZE 409600
#define RAM_SIZE          800

struct x86Buf
{
    BYTE*   buf      ;
    size_t  buf_size ;
    size_t  prog_size;
    size_t  index    ;
    ADDRESS RAM_addr ;
};

void PasteByteInBuf(x86Buf* x86_buf, BYTE byte);
void PasteNBytesInBuf(x86Buf* x86_buf, const BYTE* bytes, size_t n_bytes);
// void FillBuf(BYTE* buf, size_t buf_size, BYTE byte);

int x86BufCtor(x86Buf* x86_buf, CtorDumpMode mode = CTOR_NDUMP_MODE);
int x86BufDtor(x86Buf* x86_buf);
int x86BufVerify(const x86Buf* x86_buf);
int IRTranslate_x86(const IR* ir, x86Buf* x86_buf);
int x86BufDump(const x86Buf* x86_buf, size_t n_bytes);

int PatchAddresses(const IR* ir, x86Buf* x86_buf, size_t* offsets);

#ifndef NDEBUG
    #define VERIFY_x86BUF(x86_buf_ptr) {ASSERT(x86_buf_ptr != nullptr); x86BufVerify(x86_buf_ptr);}
#else
    #define VERIFY_x86BUF(x86_buf_ptr)
#endif

#endif

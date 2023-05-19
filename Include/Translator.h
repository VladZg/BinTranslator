#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "../Config.h"
#include "./Defines.h"
#include "./IR.h"

#define MAX_PROGRAMM_SIZE 33554432

struct x86Buf
{
    BYTE*  buf  ;
    size_t size ;
    size_t index;
};

void PasteInBuf(BYTE* buf, size_t* buf_index, BYTE byte);
void FillBuf(BYTE* buf, size_t buf_size, BYTE byte);
void EndBuf(BYTE* buf, size_t* buf_index);

void print_buf(const BYTE* buf, int buf_size);

int x86BufCtor(x86Buf* x86_buf, CtorDumpMode mode = CTOR_NDUMP_MODE);
int x86BufDtor(x86Buf* x86_buf);
int x86BufVerify(x86Buf x86_buf);
int IRTranslate_x86(const IR* ir, x86Buf* x86_buf);
int x86BufDump(x86Buf x86_buf);

#ifndef NDEBUG
    #define VERIFY_x86BUF(x86_buf) {x86BufVerify(x86_buf);}
#else
    #define VERIFY_x86BUF(x86_buf_ptr)
#endif

#endif

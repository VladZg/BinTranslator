#include "../Config.h"
#include <stdlib.h>
#include <cstring>
#include "../Include/Assert.h"
#include "../Include/Defines.h"
#include "../Include/IR.h"

void PasteInBuf(BYTE* buf, int buf_index, BYTE byte)
{
    buf[buf_index++] = byte;
}

void FillBuf(BYTE* buf, int buf_size, BYTE byte)
{
    memset(buf, buf_size, byte);
}

void EndBuf(BYTE* buf, int buf_index)
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

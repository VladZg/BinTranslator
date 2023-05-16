#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "../Config.h"
#include "./Defines.h"

void PasteInBuf(BYTE* buf, int buf_index, BYTE byte);
void FillBuf(BYTE* buf, int buf_size, BYTE byte);
void EndBuf(BYTE* buf, int buf_index);

void print_buf(const BYTE* buf, int buf_size);

#endif

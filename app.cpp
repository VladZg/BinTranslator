#include "./Config.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <limits.h>
#include "./Include/Assert.h"
#include "./Include/Defines.h"
#include "./Processor/Include/Constants.h"
#include "./Processor/Include/TechInfo.h"
#include "./Processor/Include/CheckFile.h"
#include "./Include/IR.h"
#include "./Include/Translator.h"

#define PASTE(byte) PasteInBuf(buf, buf_index, byte)

const char  FILENAME_INPUT_DEFAULT[]  = "./Source.exe";
const char* FILENAME_INPUT            = nullptr;

int main(const int argc, const char** argv)
{
    if (!CheckFile(argc, argv, &FILENAME_INPUT))
        FILENAME_INPUT = FILENAME_INPUT_DEFAULT;

    FILE* bin_file = fopen(FILENAME_INPUT, "rb");
    ASSERT(bin_file != nullptr);

    IR ir = {};
    IRCtor(&ir, bin_file, CTOR_DUMP_MODE);

    PatchIR(&ir, bin_file);

    IRDtor(&ir);

//     int buf_size = PAGESIZE;
//     BYTE* buf = (BYTE*) aligned_alloc(PAGESIZE, buf_size);
//     ASSERT(buf != nullptr);
//     int mprotect_res = mprotect(buf, buf_size, PROT_READ | PROT_WRITE | PROT_EXEC);
//     ASSERT(mprotect_res == 0);
//
//     FillBuf(buf, buf_size, 0x00);
//
//     int buf_index = 0;
//
//     PASTE(0x60);    // pusha
//     PASTE(0x61);    // popa
//     PASTE(0xC3);    // retn
//     // print_buf(buf, buf_size);
//
//     ASSERT(mprotect_res == 0);
//     void (*func)() = (void (*)())(buf);
//     func();
//
//     free(buf);

    return 1;
}

// https://www.cyberforum.ru/win-api/thread1720745.html


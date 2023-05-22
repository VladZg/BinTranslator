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

const char  FILENAME_INPUT_DEFAULT[]  = "./Source.exe";
const char* FILENAME_INPUT            = nullptr;

#define MAX(a, b) ((a) >= (b) ? a : b)

int Execute_x86Code(const x86Buf* x86_buf)
{
    ASSERT(x86_buf != nullptr);

    int mprotect_res = mprotect(x86_buf->buf, x86_buf->prog_size, PROT_EXEC);
    ASSERT(mprotect_res == 0);

    void (*exec_code)() = (void (*)())(x86_buf->buf);
    exec_code();

    mprotect_res = mprotect(x86_buf->buf, x86_buf->buf_size, PROT_READ | PROT_WRITE);
    ASSERT(mprotect_res == 0);

    return 1;
}

int main(const int argc, const char** argv)
{
    if (!CheckFile(argc, argv, &FILENAME_INPUT))
        FILENAME_INPUT = FILENAME_INPUT_DEFAULT;

    FILE* bin_file = fopen(FILENAME_INPUT, "rb");
    ASSERT(bin_file != nullptr);

    IR ir = {};
    IRCtor(&ir, bin_file);
    fclose(bin_file);

    PatchIR(&ir);
    IRDump(&ir);

    x86Buf x86_buf = {};
    x86BufCtor(&x86_buf);

    IRTranslate_x86(&ir, &x86_buf);
    x86BufDump(&x86_buf, MAX(256, x86_buf.prog_size+1));

    Execute_x86Code(&x86_buf);

    IRDtor(&ir);
    x86BufDtor(&x86_buf);

    return 1;
}


#include "./Config.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <limits.h>
#include "./Include/Assert.h"

#ifndef PAGESIZE
#define PAGESIZE 4096
#endif

typedef unsigned char BYTE;

int main()
{
    BYTE* buf = (BYTE*) malloc(10);
    ASSERT(buf != nullptr);
    buf = (BYTE*)(((long int) buf + PAGESIZE-1) & ~(PAGESIZE-1));   // выравнивание по длине страницы
    int mprotect_res = mprotect(buf, 16, PROT_EXEC);
    ASSERT(mprotect_res != 0);

    return 1;
}

// https://www.cyberforum.ru/win-api/thread1720745.html


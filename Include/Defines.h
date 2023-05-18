#ifndef DEFINES_H
#define DEFINES_H

#include "../Config.h"

#ifndef PAGESIZE
#define PAGESIZE 4096
#endif

typedef unsigned char BYTE;
typedef long unsigned int ADDRESS;

#define FUNC_NAME(cmd)                      \
{                                           \
    printf("%s:\n", __PRETTY_FUNCTION__);   \
    cmd                                     \
    printf("\n");                           \
}

#endif

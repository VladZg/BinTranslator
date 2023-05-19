#ifndef DEFINES_H
#define DEFINES_H

#include "../Config.h"
#include <stdint.h>
#include ".././Processor/Libs/Stack/Include/ColourConsts.h"

#ifndef PAGESIZE
#define PAGESIZE 4096
#endif

typedef uint8_t  BYTE   ;
typedef uint64_t ADDRESS;

#define FUNC_NAME(cmd)                                   \
{                                                        \
    printf(KCYN "%s" KNRM ":\n", __PRETTY_FUNCTION__);   \
    cmd                                                  \
    printf("\n");                                        \
}

#endif

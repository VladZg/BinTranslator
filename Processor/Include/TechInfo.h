#ifndef TECHINFO_H
#define TECHINFO_H

#include <stdlib.h>

struct TechInfo
{
    int filecode ;
    int version  ;
    int code_size;
};

struct Label
{
    char* name;
    int   value;
};

#endif

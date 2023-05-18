#ifndef IR_H
#define IR_H

#include "../Config.h"
// #include <stdlib.h>
#include "../Processor/Include/Constants.h"
#include "../Processor/Include/TechInfo.h"

enum ArgLocation
{
    STACK   ,
    REG     ,
    MEM     ,
};

struct Command
{
    size_t      pc      ;
    int         cmd_code;
    size_t      n_args  ;
    int*        args    ;
    ArgLocation arg_loc ;
};

struct IR
{
    TechInfo    info    ;
    Command*    commands;
    size_t      n_cmds  ;
};

enum IRStatusCode
{
    IR_OK_STATUS    ,
    IR_ERR_STATUS   ,
};

enum CtorDumpMode
{
    CTOR_DUMP_MODE  ,
    CTOR_NDUMP_MODE ,
};

int TechInfoCtor(TechInfo* tech_info, FILE* bin_file, CtorDumpMode mode = CTOR_NDUMP_MODE);
int VerifyTechInfo(TechInfo tech_info);
int TechInfoDtor(TechInfo* tech_info);
void TechInfoDump(TechInfo tech_info);

int IRCtor(IR* ir, FILE* bin_file, CtorDumpMode mode = CTOR_NDUMP_MODE);
IRStatusCode IRVerify(IR ir);
int IRDtor(IR* ir);
void IRDump(IR ir);
int PatchIR(const IR* ir, FILE* bin_file);

#endif

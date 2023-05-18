#ifndef IR_H
#define IR_H

#include "../Config.h"
// #include <stdlib.h>
#include "../Processor/Include/Constants.h"
#include "../Processor/Include/TechInfo.h"
#include "./Defines.h"

enum ArgLocation
{
    ARG_STACK_LOC   ,
    ARG_REG_LOC     ,
    ARG_MEM_LOC     ,
};

struct Command
{
    size_t      pc      ;
    int         cmd_code;
    size_t      n_args  ;
    int         args[2]  ;
    ArgLocation arg_loc ;
};

struct IR
{
    TechInfo    info    ;
    BYTE*       bin_code;
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

void CommandDump(Command command);
int PatchCommand(Command* command, const BYTE* bin_code, size_t pc);
int PatchIR(IR* ir);

#endif

#ifndef IR_H
#define IR_H

#include "../Config.h"
// #include <stdlib.h>
#include "./Assert.h"
#include "../Processor/Include/Constants.h"
#include "../Processor/Include/TechInfo.h"
#include "./Defines.h"

#ifndef NDEBUG
    #define VERIFY_TECH_INFO(tech_info) {VerifyTechInfo(tech_info);}
    #define VERIFY_IR(       ir_ptr   ) {ASSERT(ir_ptr != nullptr); IRVerify(ir_ptr);}
#else
    #define VERIFY_TECH_INFO(tech_info) {}
    #define VERIFY_IR(       ir_ptr   ) {}
#endif

enum ArgType
{
    ARG_NUM_TYPE     ,
    ARG_REG_TYPE     ,
    ARG_ADDR_TYPE    ,
};

enum ArgLocation
{
    ARG_NO_LOC      ,
    ARG_STACK_LOC   ,
    ARG_MEM_LOC     ,
    ARG_REG_LOC     ,
};

struct Argument
{
    int         value;
    ArgType     type ;
};

struct Command
{
    size_t       pc      ;
    int          cmd_code;
    size_t       n_args  ;
    Argument     args[2] ;
    ArgLocation  loc     ;
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

enum HighlightDumpMode
{
    LIGHT_DUMP_MODE ,
    NLIGHT_DUMP_MODE,
};

int TechInfoCtor(TechInfo* tech_info, FILE* bin_file, CtorDumpMode mode = CTOR_NDUMP_MODE);
int VerifyTechInfo(TechInfo tech_info);
int TechInfoDtor(TechInfo* tech_info);
void TechInfoDump(TechInfo tech_info);

int IRCtor(IR* ir, FILE* bin_file, CtorDumpMode mode = CTOR_NDUMP_MODE);
IRStatusCode IRVerify(const IR* ir);
int IRDtor(IR* ir);
void IRDump(const IR* ir);

void BinCodeDump(const BYTE* bin_code, size_t n_bytes, const char* tabulation, HighlightDumpMode mode = NLIGHT_DUMP_MODE, size_t highlighted_index = -1, BYTE highlighted_symbol = 0);
void CommandDump(Command command);

int PatchCommand(Command* command, const BYTE* bin_code, size_t pc);
int PatchIR(IR* ir);

#endif

#include "../Config.h"
#include <stdlib.h>
#include <cstring>
#include "../Include/Assert.h"
#include "../Include/Defines.h"
#include "../Processor/Include/Constants.h"
#include "../Processor/Include/TechInfo.h"
#include "../Include/IR.h"

#ifndef NDEBUG
    #define VERIFY_TECH_INFO(tech_info) {VerifyTechInfo(tech_info);}
    #define VERIFY_IR(       ir       ) {IRVerify(ir);             }
#else
    #define VERIFY_TECH_INFO(tech_info) {}
    #define VERIFY_IR(       ir       ) {}
#endif

int TechInfoCtor(TechInfo* tech_info, FILE* bin_file, CtorDumpMode mode)
{
    ASSERT(tech_info != nullptr);
    ASSERT(bin_file != nullptr);

    fread(tech_info, sizeof(int), TECH_INFO_SIZE, bin_file);
    VerifyTechInfo(*tech_info);

    if (mode == CTOR_DUMP_MODE) TechInfoDump(*tech_info);

    return 1;
}

int TechInfoDtor(TechInfo* tech_info)
{
    if (tech_info == nullptr) return 1;

    VerifyTechInfo(*tech_info);

    *tech_info = {};
    tech_info = nullptr;

    return 1;
}

int VerifyTechInfo(TechInfo tech_info)
{
    if (tech_info.filecode != CP_FILECODE)
        FUNC_NAME(printf("Wrong format of binary file!\n");)

    else if (tech_info.version != CMD_VERSION)
        FUNC_NAME(printf("Wrong command version!\n");)

    else if (tech_info.code_size < 0)
        FUNC_NAME(printf("Wrong number of commands!\n");)

    else
        return 1;

    return 0;
}

void TechInfoDump(TechInfo tech_info)
{
    VERIFY_TECH_INFO(tech_info)

    FUNC_NAME(
    printf("    code size: %d  \n", tech_info.code_size);
    printf("    filecode : %d  \n", tech_info.filecode );
    printf("    version  : %d\n\n", tech_info.version  );)
}

int IRCtor(IR* ir, FILE* bin_file, CtorDumpMode mode)
{
    ASSERT(ir != nullptr);
    ASSERT(bin_file != nullptr);

    TechInfoCtor(&ir->info, bin_file, CTOR_NDUMP_MODE);
    ir->n_cmds = 0;
    ir->commands = (Command*) calloc(ir->info.code_size, sizeof(Command));

    if (mode == CTOR_DUMP_MODE) IRDump(*ir);

    return 1;
}

IRStatusCode IRVerify(IR ir)
{
    if (ir.commands = nullptr)
        FUNC_NAME(printf("IR commands pointer is nullptr\n");)

    else if (!VerifyTechInfo(ir.info))
        FUNC_NAME(printf("Technical info is wrong\n");)

    else return IR_OK_STATUS;

    return IR_ERR_STATUS;
}

int IRDtor(IR* ir)
{
    if (ir == nullptr) return 1;

    IRVerify(*ir);

    TechInfoDtor(&ir->info);
    ir->n_cmds = 0;
    free(ir->commands);
    ir->commands = nullptr;

    *ir = {};
    ir = nullptr;

    return 1;
}

void IRDump(IR ir)
{
    VERIFY_IR(ir)

    FUNC_NAME(
    printf("    info     = {               \n"                   );
    printf("                code_size = %d \n", ir.info.code_size);
    printf("                filecode  = %d \n", ir.info.filecode );
    printf("                version   = %d \n", ir.info.version  );
    printf("               }               \n"                   );
    printf("    n_cmds   = %u              \n", ir.n_cmds        );
    printf("    commands = {}              \n"                   );)
}

int PatchIR(const IR* ir, FILE* bin_file)
{
    ASSERT(ir != nullptr);
    ASSERT(bin_file != nullptr);



    return 1;
}

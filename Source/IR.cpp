#include "../Config.h"
#include <stdlib.h>
#include <cstring>
#include "../Processor/Libs/Stack/Include/ColourConsts.h"
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

    ir->bin_code = (BYTE*) calloc(ir->info.code_size, sizeof(BYTE));
    ASSERT(ir->bin_code != nullptr);
    fread(ir->bin_code, sizeof(BYTE), ir->info.code_size, bin_file);

    ir->n_cmds = 0;
    ir->commands = (Command*) calloc(ir->info.code_size, sizeof(Command));
    ASSERT(ir->commands != nullptr);

    if (mode == CTOR_DUMP_MODE) IRDump(*ir);

    return 1;
}

IRStatusCode IRVerify(IR ir)
{
    if (ir.commands = nullptr)
        FUNC_NAME(printf("Commands pointer is nullptr\n");)

    else if (ir.bin_code = nullptr)
        FUNC_NAME(printf("Bin code pointer is nullptr\n");)

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
    free(ir->bin_code);
    ir->bin_code = nullptr;

    *ir = {};
    ir = nullptr;

    return 1;
}

static void BinCodeDump(const BYTE* bin_code, size_t n_bytes, const char* tabulation)
{
    size_t str_len = 32;

    for (size_t byte_index = 0; byte_index < n_bytes; byte_index++)
    {
        printf("%s", byte_index % str_len == 0 ? tabulation : "");
        printf("%02X %s", bin_code[byte_index], (byte_index + 1) % str_len == 0 ? "\n" : "");
    }

    printf("\n");
}

void IRDump(IR ir)
{
    VERIFY_IR(ir)

    FUNC_NAME(
    printf("    info     = {                               \n"                   );
    printf("                code_size = " KYEL "%d" KNRM " \n", ir.info.code_size);
    printf("                filecode  = " KYEL "%d" KNRM " \n", ir.info.filecode );
    printf("                version   = " KYEL "%d" KNRM " \n", ir.info.version  );
    printf("               }                               \n"                   );
    printf("    n_cmds   = " KYEL "%u" KNRM "              \n", ir.n_cmds        );
    printf("    commands = {                               \n"                   );

    for (size_t cmd_index = 0; cmd_index < ir.n_cmds; cmd_index++)
    {
        printf("                [" KMAG "%4u" KNRM "]", cmd_index);
        CommandDump(ir.commands[cmd_index]);
    }

    printf("               }               \n"                   );
    printf("    bin_code = {               \n"                   );
    BinCodeDump(ir.bin_code, ir.info.code_size, "                ");
    printf("               }               \n"                   );
    )
}

#define DEF_CMD(name, num, arg, ...)        \
    case(num): return #name;

#define DEF_JMP(name, num, condition, ...)  \
    case(num): return #name;

#define DEF_DUMP(name, num)                 \
    case(num): return #name;

const char* TranslateCmdCodeToCmdName(CommandCode cmd_code)
{
    switch(cmd_code)
    {
        #include "../Processor/Include/Cmd.h"

        default: return "(EMPTY)";
    }
}

#undef DEF_DUMP
#undef DEF_JMP
#undef DEF_CMD

static const char* GetArgTypeName(ArgType type)
{
    switch(type)
    {
        case (ARG_NUM_TYPE ): return "  NUMBER";
        case (ARG_REG_TYPE ): return "REGISTER";
        case (ARG_ADDR_TYPE): return " ADDRESS";

        default: "   EMPTY";
    }
}

static const char* GetArgLocName(ArgLocation loc)
{
    switch(loc)
    {
        case (ARG_STACK_LOC ): return "   STACK";
        case (ARG_MEM_LOC   ): return "  MEMORY";
        case (ARG_REG_LOC   ): return "REGISTER";
        case (ARG_NO_LOC    ): return "  NO LOC";

        default: "   EMPTY";
    }
}

#define DEF_REG( name, num )              \
    case REG_##name: return "   " #name;

static const char* GetRegName(RegistersInfo value)
{
    switch(value)
    {
        #include "../Processor/Include/Reg.h"

        default: return "  NONE";
    }
}

#undef DEF_REG

static void PrintCommandArgValue(Argument arg)
{
    switch(arg.type)
    {
        case ARG_NUM_TYPE:
            printf(KGRN "%6d" KNRM, arg.value); break;

        case ARG_REG_TYPE:
            printf(KGRN "%s" KNRM, GetRegName((RegistersInfo) arg.value)); break;

        case ARG_ADDR_TYPE:
            printf(KGRN "%6X" KNRM, arg.value); break;

        default:
            printf("");
    }
}

static void CommandArgsDump(Command command)
{
    printf("n_args = " KRED "%d" KNRM ", loc = " KYEL "%s" KNRM ", args = {", command.n_args, GetArgLocName(command.loc));

    for (size_t arg_index = 0; arg_index < command.n_args; arg_index++)
    {
        printf("(" KMAG "%s" KNRM ": ", GetArgTypeName(command.args[arg_index].type));
        PrintCommandArgValue(command.args[arg_index]);
        printf("), ");
    }

    if (command.n_args > 0) printf("\b\b");
}

void CommandDump(Command command)
{
    printf(KYEL "%15s" KNRM ": pc = " KBLU "%5u" KNRM ", cmd_code = " KCYN "%2u" KNRM ", ", TranslateCmdCodeToCmdName((CommandCode) command.cmd_code), command.pc, command.cmd_code);
    CommandArgsDump(command);
    printf("}\n");
}

#define DEF_CMD(name, num, arg, ...)
#define DEF_JMP(name, num, condition, ...)                                                                                          \
{                                                                                                                                   \
    if (cmd_num_code == num)                                                                                                        \
    {                                                                                                                               \
        command->args[0] = {*(int*)(cmd_code + sizeof(BYTE)), ARG_ADDR_TYPE};                                                       \
        command->loc = ARG_NO_LOC;                                                                                                  \
        command->n_args++;                                                                                                          \
    }                                                                                                                               \
}

#define DEF_DUMP(name, num)                                                                                                         \
{                                                                                                                                   \
    if (cmd_num_code == num)                                                                                                        \
    {                                                                                                                               \
        command->n_args += 2;                                                                                                       \
        command->args[0] = {*(int*)(cmd_code + sizeof(BYTE) + 0*sizeof(int)), ARG_NUM_TYPE};                                        \
        command->args[1] = {*(int*)(cmd_code + sizeof(BYTE) + 1*sizeof(int)), ARG_NUM_TYPE};                                        \
    }                                                                                                                               \
}

int PatchCommand(Command* command, const BYTE* bin_code, size_t pc)
{
    ASSERT(command != nullptr);
    // printf("%2X ", *bin_code_ptr);

    const BYTE* cmd_code = bin_code + pc;
    BYTE cmd_num_code = *cmd_code & CMD_CODE_MASK;
    // printf("%X --> %X\n", bin_code[pc], cmd_code);

    command->n_args = 0;

    if (*cmd_code & ARG_IMMED)
    {
        command->args[0] = {*(int*)(cmd_code + sizeof(BYTE)), ARG_NUM_TYPE};
        command->n_args++;
    }

    if (*cmd_code & ARG_REG)
    {
        command->args[command->n_args] = {*(int*)(cmd_code + sizeof(BYTE) + (command->n_args)*sizeof(int)), ARG_REG_TYPE};
        command->n_args++;
        command->loc = ARG_REG_LOC;
    }

    if (*cmd_code & ARG_MEM)
        command->loc = ARG_MEM_LOC;

    if (cmd_num_code == CMD_PUSH)
        command->loc = ARG_STACK_LOC;

    if (command->n_args == 0)
        command->loc = ARG_NO_LOC;

    #include "../Processor/Include/Cmd.h"

    command->pc = pc;
    command->cmd_code = cmd_num_code;
    // CommandDump(commands[pc]);

    return 1;
}

#undef DEF_DUMP
#undef DEF_JMP
#undef DEF_CMD

int PatchIR(IR* ir)
{
    ASSERT(ir != nullptr);
    VERIFY_IR(*ir)

    size_t pc = 0;
    size_t cmd_index = 0;
    ir->n_cmds = 0;

    while (pc < ir->info.code_size)
    {
        PatchCommand(ir->commands + cmd_index, ir->bin_code, pc);
        pc += 1 + ir->commands[cmd_index].n_args * sizeof(int);
        ir->n_cmds++;
        cmd_index++;
    }

    // IRDump(*ir);

    return 1;
}

ASM_DIR = ./AsmProgs/
filename = test

NO_OUT = >/dev/null

all: run

run: asm_compile files_rename bin_translate

asm_compile:
	@make -f ./Processor/Run ROOT_DIR=./Processor/ filename=./AsmProgs/$(filename).asm $(NO_OUT)

files_rename:
	@mv -v ./Source.dsm ./$(filename).dsm $(NO_OUT)
	@mv -v ./Source.exe ./$(filename).exe $(NO_OUT)
	@mv -v ./Source.lst ./$(filename).lst $(NO_OUT)

bin_translate:

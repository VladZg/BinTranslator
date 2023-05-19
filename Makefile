TARGET = ./app
CFLAGS = -fsanitize=address -O3 -w
NO_OUT = >/dev/null

SOURCE = ./app.cpp ./Source/Translator.cpp ./Source/IR.cpp

filename = test

all: recompile_processor compile_app run_asm_compilation run_app

compile_app:
	@g++ $(SOURCE) $(CFLAGS) -o $(TARGET) $(NO_OUT)

recompile_processor:
	@make -f ./Processor/Compile ROOT_DIR=./Processor/ $(NO_OUT)

run_asm_compilation:
	@make -f Run filename=$(filename) $(NO_OUT)

run_app:
	@$(TARGET) ./$(filename).exe

BYTE_CHECK_DIR 		= ./ByteCheck
BYTE_CHECK_TARGET 	= $(BYTE_CHECK_DIR)/byte_check

check:
	@nasm -f elf64 $(BYTE_CHECK_TARGET).s -l $(BYTE_CHECK_TARGET).lst -o $(BYTE_CHECK_TARGET).o
	@g++ -no-pie -s -Wno-format $(BYTE_CHECK_TARGET).o -o $(BYTE_CHECK_TARGET)
	@$(BYTE_CHECK_TARGET)

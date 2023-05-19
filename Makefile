TARGET = ./app
CFLAGS = -fsanitize=leak -O3 -w
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

TARGET = ./app
CFLAGS = -fsanitize=leak -O3
NO_OUT = >/dev/null

SOURCE = ./app.cpp ./Source/Translator.cpp

all: recompile_processor compile run

compile:
	@g++ $(SOURCE) $(CFLAGS) -o $(TARGET)

recompile_processor:
	@make -f ./Processor/Compile ROOT_DIR=./Processor/ $(NO_OUT)

run:
	@$(TARGET)

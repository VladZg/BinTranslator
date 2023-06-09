#include <stdio.h>

const char HELP_MESSAGE[] = ""
"   Список команд версии 4 с их кодами для целочисленного процессора:"
"\n"
"\n       0000 - HLT  - окончание программы, очистка буферов"
"\n                   - нет аргументов"
"\n"
"\n       0001 - PUSH - введённое число кладётся в стек"
"\n                   - от 1 до 2 аргументов указанного типа и в установленном порядке:"
"\n                       - число"
"\n                       - имя регистра"
"\n                       - число + число"
"\n                       - число + имя регистра"
"\n                       * вышеперечисленные аргументы можно указывать под общей скобкой [...], будет браться элемент оперативной памяти по указанному адресу"
"\n"
"\n       0002 - ADD  - сложение предпоследнего числа из стека с последним, оба удаляются из стека, результат кладётся в стек"
"\n                   - нет аргументов"
"\n"
"\n       0003 - SUB  - вычитание последнего числа из стека из последнего, оба удаляются из стека, результат кладётся в стек"
"\n                   - нет аргументов"
"\n"
"\n       0004 - MUL  - умножение предпоследнего числа из стека на последнее, оба удаляются из стека, результат кладётся в стек"
"\n                   - нет аргументов"
"\n"
"\n       0005 - DIV  - деление предпоследнего числа из стека на последнее, оба удаляются из стека, результат кладётся в стек"
"\n                   - нет аргументов"
"\n"
"\n       0006 - OUT  - вывод последнего числа из стека и йудаление его оттуда"
"\n                   - нет аргументов"
"\n"
"\n       0007 - DUMP - вывод инфромации об исполняемом файле"
"\n                   - 2 целочисленных аргумента в установленном порядке:"
"\n                       - 1) минимальный ip, начиная с которого отображается информация"
"\n                       - 2) максимальный ip, до которого отображается информация"
"\n                       * в слечае неправильного указания аргументов выводится полная информация об исполняемом файле"
"\n"
"\n       0008 - IN   - после ввода числа с клавиатуры, оно будет помещено в стек"
"\n                   - нет аргументов"
"\n"
"\n       0009 - POP  - присвоение указанному элементу значения последнего элемента стека"
"\n                   - от 1 до 2 аргументов указанного типа и в установленном порядке:"
"\n                       - имя регистра"
"\n                       - [число + число]"
"\n                       - [число + имя регистра]"
"\n                       - [число]"
"\n                       - [имя регистра]"
"\n                       * при указании аргументов в квадратных скобках [...] значение последнего элемента стека присваивается ячейке оперативной памяти по этому адресу"
"\n"
"\n       0010 - JMP  - переход на выполнения кода с места, где установлена метка"
"\n                   - 1 строковый или целочисленный аргумент:"
"\n                       - номер ip, на который нужно перейти"
"\n                       - название метки вида:"
"\n                           - :число"
"\n                           - :имя"
"\n                           - имя"
"\n"
"\n       0011 - JB   - jmp, если предпоследний элемент стека <  последнего, элементы удаляются из стека в любом случае"
"\n                   - 1 строковый или целочисленный аргумент:"
"\n                       - номер ip, на который нужно перейти"
"\n                       - название метки вида:"
"\n                           - :число"
"\n                           - :имя"
"\n                           - имя"
"\n"
"\n       0012 - JBE  - jmp, если предпоследний элемент стека <= последнего, элементы удаляются из стека в любом случае"
"\n                   - 1 строковый или целочисленный аргумент:"
"\n                       - номер ip, на который нужно перейти"
"\n                       - название метки вида:"
"\n                           - :число"
"\n                           - :имя"
"\n                           - имя"
"\n"
"\n       0013 - JA   - jmp, если предпоследний элемент стека >  последнего, элементы удаляются из стека в любом случае"
"\n                   - 1 строковый или целочисленный аргумент:"
"\n                       - номер ip, на который нужно перейти"
"\n                       - название метки вида:"
"\n                           - :число"
"\n                           - :имя"
"\n                           - имя"
"\n"
"\n       0014 - JAE  - jmp, если предпоследний элемент стека >= последнего, элементы удаляются из стека в любом случае"
"\n                   - 1 строковый или целочисленный аргумент:"
"\n                       - номер ip, на который нужно перейти"
"\n                       - название метки вида:"
"\n                           - :число"
"\n                           - :имя"
"\n                           - имя"
"\n"
"\n       0015 - JE   - jmp, если предпоследний элемент стека =  последнеиу, элементы удаляются из стека в любом случае"
"\n                   - 1 строковый или целочисленный аргумент:"
"\n                       - номер ip, на который нужно перейти"
"\n                       - название метки вида:"
"\n                           - :число"
"\n                           - :имя"
"\n                           - имя"
"\n"
"\n       0016 - JNE  - jmp, если предпоследний элемент стека != последнему, элементы удаляются из стека в любом случае"
"\n                   - 1 строковый или целочисленный аргумент:"
"\n                       - номер ip, на который нужно перейти"
"\n                       - название метки вида:"
"\n                           - :число"
"\n                           - :имя"
"\n                           - имя"
"\n"
"\n       0017 - JF   - jmp, если день выполнения программы - пятница"
"\n                   - 1 строковый или целочисленный аргумент:"
"\n                       - номер ip, на который нужно перейти"
"\n                       - название метки вида:"
"\n                           - :число"
"\n                           - :имя"
"\n                           - имя"
"\n"
"\n       0018 - RUS  - печать флага России!!!"
"\n                   - нет аргументов"
"\n"
"\n       0019 - PRINT- числа в формате \"ARGUMENT: <int>\""
"\n                   - аргументы, как у PUSH"
"\n"
"\n       0020 - RAM  - вывод состояния оперативной памяти"
"\n                   - нет аргументов"
"\n"
"\n       0021 - CALL - вызов функции по метке"
"\n                   - аргументы, как у JMP"
"\n"
"\n       0022 - RET  - возвращение к части кода после вызванной функции"
"\n                   - нет аргументов";

void PrintHelp(const int argc, const char** argv)
{
    if (argc > 1)
    {
        if (!strcmp(argv[1], "--h"))
        {
            fprintf(stderr, "%s\n\n", HELP_MESSAGE);
            exit(1);
        }
    }
}

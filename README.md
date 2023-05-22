# Бинарный транслятор

В данном проекте мной был создан компилятор для собственного диалекта ассемблера, который транслирует программы, написанные на этом языке, в исполняемый машинный код архитектуры Intel x86-64.

## Принципы работы

Начальной стадией является написание программы на языке программирования "Kryuzhovnik" (ссылка на repo)

### Программа на диалекте ассемблера

Диалект ассемблера (ссылка на repo) повзоляет написать программу.

### Промежуточное представление (IR)

Представление программы в описанном бинарном файле сложно напрямую транслировать в код на x86-64. Поэтому используется промежуточное представление (IR) - структура данных, содержащая всю информацию об исполняемой программе:

```C++
    struct IR
    {
        TechInfo    info    ;
        BYTE*       bin_code;
        Command*    commands;
        size_t      n_cmds  ;
    };
```

Каждая команда в массиве ``commands`` также является структурой, содержащей информацию о своём типе, аргументах:

```C++
    struct Command
    {
        size_t       pc      ;
        int          cmd_code;
        size_t       n_args  ;
        Argument     args[2] ;
        ArgLocation  loc     ;
    };
```

Таким образом, после обработки парсером в IR программа имеет следующий вид:

![IR dump](./Pictures/IRDump.png)

### Трансляция в исполняемый бинарный код x86-64



## Оценка производительности

    Сравним скорости исполнения программы на собственном эмуляторе процессора и на реальном процессоре при её бинарной трансляции:



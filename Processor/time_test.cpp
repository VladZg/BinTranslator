#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define GetTime(cmds)                                                       \
{                                                                           \
    clock_t timer_start = clock();                                          \
    cmds                                                                    \
    clock_t timer_end = clock();                                            \
    double seconds = ((double) (timer_end - timer_start));                  \
    printf("%lf seconds\n", seconds);                                       \
}

int main()
{
    GetTime(
        for (int i = 0; i < 10000; i++)
            system("make -f Run run");
    )

    return 1;
}

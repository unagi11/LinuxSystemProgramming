#include <stdio.h>
#include <stdlib.h>

void ssu_out(void);
int main(int argc, char const *argv[])
{
    // ssu_out함수를 atexit으로 등록한다.
    if (atexit(ssu_out))
    {
        fprintf(stderr, "atexit error\n");
        exit(1);
    }

    exit(0);
}

void ssu_out(void) 
{
    printf("atexit succeeded!\n");
}
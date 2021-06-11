#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void ssu_charatatime(char * str);

int main(int argc, char const *argv[])
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if (pid == 0)
        ssu_charatatime("output from child\n");
    else
        ssu_charatatime("output from parent\n");
    
    return 0;
}

static void ssu_charatatime(char *str)
{
    char *ptr;
    int print_char;

    setbuf(stdout, NULL);

    // 약간씩 기다리면서 한 글자씩 출력
    for (ptr = str; (print_char = *ptr++) != 0;)
    {
        putc(print_char, stdout);
        usleep(10);
    }
}
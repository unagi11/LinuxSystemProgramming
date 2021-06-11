#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    pid_t pid;
    char character, first, last;
    long i;

    if ((pid = fork()) > 0)
    {
        first = 'A';
        last = 'Z';
    }
    else if (pid == 0) 
    {
        first = 'a';
        last = 'z';
    }
    else
    {
        fprintf(stderr, "%s\n", argv[0]);
        exit(1);
    }

    for (character = first; character <= last; character++)
    {
        for (int i = 0; i < 100000; i++);

        write(1, &character, 1);        
    }

    printf("\n");
    return 0;
}

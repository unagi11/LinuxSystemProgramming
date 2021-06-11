#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    pid_t pgid;
    pid_t pid;

    if (argc < 2)
    {
        fprintf(stderr, "usage: %s <pid No.>\n", argv[0]);
        exit(1);
    }

    pid = getpid();
    pgid = getpgid(atoi(argv[1]));
    printf("pid: %d, pgid: %d\n", pid, pgid);
    
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    pid_t pgid;
    pid_t pid;

    pid = getpid();
    pgid = getpgrp();
    printf("pid: %d, pgid: %d\n", pid, pgid);

    return 0;
}
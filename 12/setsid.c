#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if (pid != 0)
        exit(1);

    printf("before pid = %d, sid = %d\n", getpid(), getsid(getpid()));
    setsid();
    printf("after pid = %d, sid = %d\n", getpid(), getsid(getpid()));

    return 0;
}

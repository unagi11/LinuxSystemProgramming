#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
    printf("Process ID             = %d\n", getpid());
    printf("Parent Process ID      = %d\n", getppid());
    printf("Real user ID           = %d\n", getuid());
    printf("Effective user ID      = %d\n", geteuid());
    printf("Real group ID          = %d\n", getgid());
    printf("Effective group ID     = %d\n", getegid());

    return 0;
}
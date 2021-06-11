#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main()
{
    pid_t pid;
    int status, ret_val;
    if ((pid = fork()) > 0)
    {
        printf("parent PID : %d \n", getpid());
        sleep(2);
        ret_val = waitpid(123456, &status, WNOHANG); // 존재하지 않는 자식 id (123456) 사용
        printf("parent is terminating %d %d %s\n", ret_val, errno, strerror(errno));
        exit(0);
    }
    else if (pid == 0)
    {
        printf("child PID : %d\n", getpid());
        sleep(4);
        printf("child terminated\n");
        exit(0);
    }
    else
    {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    exit(0);
}

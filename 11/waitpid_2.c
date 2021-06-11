#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int status, ret_val;
    if ((pid = fork()) > 0)
    {
        printf("parent PID : %d \n", getpid());
        sleep(2);
        ret_val = waitpid(pid, &status, WNOHANG);
        // WNOHANG걸려있어서, 자식프로세스 종료 안기다림.
        printf("parent is waiting and terminated %d %d\n", ret_val, WIFEXITED(status));
        // 자식이 종료되지 않았기 때문에 ret_val은 0이된다. IFEXITED도 false다.
        exit(0);
    }
    else if (pid == 0)
    {
        printf("child PID: %d\n", getpid());
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
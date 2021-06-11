#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#define NPROCESSES 50000

void do_nothing()
{
    int i;
    i = 0;
}
int main(int argc, char *argv[])
{
    int pid, j, status;
    struct timeval start_time, end_time;
    double delay_time;
    gettimeofday(&start_time, NULL);
    for (j = 0; j < NPROCESSES; j++)
    {
        if ((pid = fork()) < 0)
        {
            printf("fork failed with error code= %d\n", pid);
            exit(0);
        }
        else if (pid == 0)
        {
            do_nothing();
            exit(0);
        }
        else
        {
            waitpid(pid, &status, 0);
        }
    }
    gettimeofday(&end_time, NULL);
    delay_time = (end_time.tv_sec - start_time.tv_sec) + ((end_time.tv_usec - start_time.tv_usec) / 1000000);
    printf("%f seconds\n", delay_time);
}
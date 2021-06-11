#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define NTHREADS 50000
void *do_nothing(void *null)
{
    int i;
    i = 0;
    pthread_exit(NULL);
}
int main(int argc, char *argv[])
{
    int rc, i, j, detachstate;
    struct timeval start_time, end_time;
    double delay_time;
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    gettimeofday(&start_time, NULL);
    for (j = 0; j < NTHREADS; j++)
    {
        rc = pthread_create(&tid, &attr, do_nothing, NULL);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        /* Wait for the thread */
        rc = pthread_join(tid, NULL);
        if (rc)
        {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }
    gettimeofday(&end_time, NULL);
    delay_time = (end_time.tv_sec - start_time.tv_sec) + ((end_time.tv_usec - start_time.tv_usec) / 1000000);
    printf("%f seconds\n", delay_time);
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
}
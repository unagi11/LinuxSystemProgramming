#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_thread(void *arg);
pthread_t tid[5];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int shared_value;

int main(void)
{
    int i, id[5];
    shared_value = 0;
    for (i = 0; i < 5; i++)
    {
        id[i] = i;
        if (pthread_create(&tid[i], NULL, ssu_thread, (void *)&id[i]) != 0)
        {
            fprintf(stderr, "pthread create error\n");
            exit(1);
        }
    }

    sleep(1);

    // pthread_cond_signal(&cond);
    // cond 조건 변수를 기다리고 있는 스레드 중의 하나를 다시 시작시킨다.
    pthread_cond_broadcast(&cond);
    // cond 조건 변수를 기다리고 있는 모든 스레드를 다시 시작시킨다.

    for (i = 0; i < 5; i++)
        pthread_join(tid[i], NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
void *ssu_thread(void *arg)
{
    int loc_id;
    loc_id = *((int *)arg);
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    // 신호를 받을때까지 기다린다.

    shared_value++;
    printf("Thread no. %d, Shared value %d\n", loc_id, shared_value);
    pthread_mutex_unlock(&mutex);

    // pthread_cond_signal(&cond);
    
}
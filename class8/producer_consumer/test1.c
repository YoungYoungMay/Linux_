//生产者与消费者模型
//基于环形队列实现
//这里单生产单消费，改进为多生产多消费_加锁
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define MAX 32

int ring[MAX];
//pthread_mutex_t lock;
sem_t sem_blank;
sem_t sem_data;

void* consumer(void* arg)
{
    int i = 0;
    while(1)
    {
        //P(sem_data)
        sem_wait(&sem_data);
        int data = ring[i];
        i++;
        i %= MAX;
        printf("consume thread %x done, data is %d\n", pthread_self(), data);
        //V(sem_blank)
        sem_post(&sem_blank);
        usleep(500000);
    }
}

void* producer(void* arg)
{
    srand((unsigned long)time(NULL));
    int i = 0;
    while(1)
    {
        //P(sem_blank)
        sem_wait(&sem_blank);
        int data = rand()%100 + 1;
        ring[i] = data;
        printf("product thread %x done, data is %d\n", pthread_self(), data);
        i++;
        i %= MAX;
        //V(sem_data)
        sem_post(&sem_data);
        usleep(1000000);
    }
}

int main()
{
    //pthread_mutex_init(&lock, NULL);
    
    pthread_t c,p;
    int ring[MAX];

    sem_init(&sem_blank, 0, MAX);
    sem_init(&sem_data, 0, 0);

    pthread_create(&c, NULL, consumer, NULL);
    pthread_create(&p, NULL, producer, NULL);
    
    pthread_join(c, NULL);
    pthread_join(p, NULL);
    
    sem_destroy(&sem_blank);
    sem_destroy(&sem_data);
    //pthread_mutex_destroy(&lock);
}

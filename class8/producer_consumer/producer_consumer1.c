//基于POSIX信号量实现生产者与消费者模型
//基于定长的循环队列实现
#include "seqqueue.c"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define MAXSIZE 20

int buf[MAXSIZE];

pthread_mutex_t mutex;//互斥锁
sem_t sem_empty;
sem_t sem_full;
SeqQueue q;


void* consumer(void* p)//消费者消费产品
{
    int num = (int)p;

    int i;
    while(1)
    {
        //队列为空时等待
        printf("%d wait buf no empty\n");
        sem_wait(&sem_empty);
        
        //不为空时进行操作
        pthread_mutex_lock(&mutex);
        //删除队首元素
        printf("%d consume %d\n", num, q.data[q.head]);
        SeqQueuePop(&q);
        pthread_mutex_unlock(&mutex);

        sem_post(&sem_full);
        sleep(1);
    }       
}

void* producer(void* p)//生产者生产产品
{
    int num = (int)p;
    int i;
    while(1)
    {
        printf("%d wait buf not full\n", num);
        sem_wait(&sem_full);//满时就等待
        pthread_mutex_lock(&mutex);
        SeqQueuePush(&q, rand()%20);
        if(q.size == 0)
            i = -1;//队列为空时
        else
            i = q.tail - 1;//让i指向刚刚插入的结点
        printf("%d product %d\n", num, q.data[i]);
        pthread_mutex_unlock(&mutex);
        sem_post(&sem_empty);//资源使用完毕，释放资源
        sleep(5);
    }
}

int main()
{
    srand((unsigned int)time(NULL));
    int i;
    pthread_t threads[8];
    //初始化
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem_full, 0, MAXSIZE);
    sem_init(&sem_empty, 0, 0);

    for(i=0; i<3; i++)//0-2生产
    {
        pthread_create(&threads[i], NULL, producer, (void*)i);
    }

    for(i=3; i<8; i++)//3-7消费
    {
        pthread_create(&threads[i], NULL, consumer, (void*)i);
    }

    for(i=0; i<8; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    return 0;
}

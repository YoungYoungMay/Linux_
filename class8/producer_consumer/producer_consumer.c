//基于条件变量实现生产者与消费者模型
//基于链表实现，空间可以动态分配
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct msg//链表一个结点的结构
{
    int num;
    struct msg* next;
}msg;

msg* head = NULL;//用一个空指针表示整个链表

pthread_mutex_t mutex;//互斥锁
pthread_cond_t cond;//条件变量
pthread_t threads[8];

void* consumer(void* p)//消费者消费->删除一个结点
{
    int num = (int)p;
    msg* mp;
    while(1)
    {
        pthread_mutex_lock(&mutex);//加锁
        //链表为空时，进行等待
        while(head == NULL)
        {
            printf("%d begin wait consume\n", num);
            pthread_cond_wait(&cond, &mutex);//等待
        }
        
        //链表不为空时，进行消费
        printf("%d end of wait\n", num);
        printf("%d begin cousume\n", num);
        //进行消费动作,删除一个结点
        mp = head;
        head = head->next;
        pthread_mutex_unlock(&mutex);//解锁
        
        //打印并释放结点
        printf("Consume %d\n", mp->num);
        free(mp);
        printf("%d end consume\n", num);
        printf("\n");
    }

}

void* producer(void* p)//生产者生产->增加一个结点
{
    msg* mp;
    int num = (int)p;
    while(1)
    {
        //要先生产，才可以消费
        printf("%d begin product\n", num);
        mp = (msg*)malloc(sizeof(msg));
        mp->num = rand()%100+1;
        printf("product is %d\n", mp->num);

        pthread_mutex_lock(&mutex);
        //将生产好的结点连到链表上
        mp->next = head;
        head = mp;
        printf("%d end of product\n", num);
        //唤醒消费者
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        
        printf("\n");
        sleep(5);
    }
}

int main()
{
    srand(time(NULL));
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
    
    int i;
    for(i=0; i<5; i++)//消费者
    {
        pthread_create(&threads[i], NULL, consumer, (void*)i);
    }

    for(i=5; i<8; i++)//生产者
    {
        pthread_create(&threads[i], NULL, producer, (void*)i);
    }

    for(i=0; i<8; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}

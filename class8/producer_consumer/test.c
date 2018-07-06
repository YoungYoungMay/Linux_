//生产者与消费者模型_条件变量
//基于带头结点的链表实现
//这里生产者慢，消费者快
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lock;//互斥锁
pthread_cond_t cond;//条件变量
//pthread_t threads[8];

typedef struct msg//链表一个结点的结构
{
    int data;
    struct msg* next;
}msg;

msg* alloc_node(int x)
{
    msg* p = (msg*)malloc(sizeof(msg));
    if(p == NULL)
    { 
        perror("malloc");
        exit(1);
    }
    p->data = x;
    p->next = NULL;
    return p;
}

void init_list(msg** head)
{
    *head = alloc_node(0);
}

void list_push(msg* head, int x)
{
    msg* p = alloc_node(x);
    p->next = head->next;
    head->next = p;
}

int isEmpty(msg* head)
{
    return head->next == NULL ? 1 : 0;
}

void list_pop(msg* head, int* data)
{
    if(!isEmpty(head))
    {
        msg* p = head->next;
        *data = head->next->data;
        head->next = p->next;
        free(p);
        p = NULL;
    }
}

void list_destroy(msg* head)
{
    int x;
    while(!isEmpty(head))
    {
        list_pop(head, &x);
    }
    free(head);
}

void list_show(msg* head)
{
    if(!isEmpty(head))
    {
        msg* p = head->next;
        while(p)
        {
            printf("%d ", p->data);
            p = p->next;
        }
        printf("\n");
    }
}

void* consumer(void* arg)
{
    msg* head = (msg*)arg;
    while(1)
    {
        int data;
        
        pthread_mutex_lock(&lock);
        //while:若消费者被异常唤醒，它会再循环回来判断一次链表是否为空，不为空时才会去消费
        //if:若用if，只要它被唤醒就去消费，但是若是异常唤醒的，链表依旧为空
        while(isEmpty(head))
        { 
            printf("no data for cousume\n");
            //这里若不用条件变量实现同步，消费者会一直占着锁一直询问，导致生存者得不到锁无法生产数据
            pthread_cond_wait(&cond, &lock);//等待的同时，也要释放锁
        }
        list_pop(head, &data);
        printf("consume done, data is %d\n", data);
        pthread_mutex_unlock(&lock);
        
    }
}

void* producer(void* arg)
{
    msg* head = (msg*)arg;
    while(1)
    {
        int data = 0;
        data = rand()%100 + 1;

        pthread_mutex_lock(&lock);
        list_push(head, data);//向链表中插入数据
        pthread_mutex_unlock(&lock);
    
        pthread_cond_signal(&cond);//唤醒消费者
        printf("product done, data is %d\n", data);
        sleep(5);
    }
}

int main()
{
    srand((unsigned long)time(NULL));

    msg* head;
    init_list(&head);
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);
    int i = 0;
    pthread_t c,p;
    pthread_create(&c, NULL, consumer, (void*)head);
    pthread_create(&p, NULL, producer, (void*)head);
    
    pthread_join(c, NULL);
    pthread_join(p, NULL);
    list_destroy(head);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
//    for(i=0; i<10; i++)
//    {
//        list_push(head, i);
//        list_show(head);
//        sleep(1);
//    }
//
//    for(i=0; i<5; ++i)
//    {
//        list_pop(head);
//        list_show(head);
//        sleep(1);
//    }
  
}

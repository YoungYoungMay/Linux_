//读写锁—创建5个读者进程，3个写者进程
//不好验证加读写锁前后的变化
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int counter = 0;
pthread_rwlock_t rwlock;

void* write_route(void* arg)
{
    int num = (int)arg;
    int t = 0;
    while(1)
    {
        pthread_rwlock_wrlock(&rwlock);
        t = counter;
        printf("write thread: %d, tid :%x, old=%d, new=%d\n", num, pthread_self(), t, ++counter);
        pthread_rwlock_unlock(&rwlock);
        
        usleep(600000);
    }
}

void* read_route(void* arg)
{
    int num = (int)arg;
    int t;
    while(1)
    {
        pthread_rwlock_rdlock(&rwlock);//对读加锁
        printf("read thread: %d, tid :%x, cur data is %d\n", num, pthread_self(), counter);
        pthread_rwlock_unlock(&rwlock);
        //若读者直接不sleep，就导致锁一直在读者这里，一直循环，导致写者写不了
        //sleep只要存在，不管几秒，0秒都可以，写者总有机会得到调度
        usleep(0);
    }
}

int main()
{
    int i;
    pthread_t tid[8];
    pthread_rwlock_init(&rwlock, NULL);

    for(i=0; i<3; ++i)//写者线程
    {
        pthread_create(tid+i, NULL, write_route, (void*)i);
    }

    for(i=3; i<8; ++i)//读者线程
    {
        pthread_create(&tid[i], NULL, read_route, (void*)i);
    }

    for(i=0; i<8; ++i)
    {
        pthread_join(tid[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock);
    return 0;
}

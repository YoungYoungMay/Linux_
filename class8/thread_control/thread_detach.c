//所有线程的pid都是进程的pid，均相同
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_run(void* arg)//传入线程创建的函数
{
    const char* msg = (const char*)arg;
    printf("%s: tid:%#x, pid:%d\n", msg, pthread_self(), getpid());
    sleep(3);
    //pthread_detach(pthread_self());//自己分离自己
    //只要线程异常，不论是否分离，都会影响进程
    int *a;
    *a = 10;
}

int main()
{
    pthread_t tid;//无符号长整型
    pthread_create(&tid, NULL, thread_run, "thread 1");
   
    //分离后再join，就会出错
    pthread_detach(tid);
    
    while(1)
    {
        printf("I am main thread\n");
        sleep(1);
    }
    ////sleep(2);
    //int ret = pthread_join(tid, NULL);//不关心线程退出信息
    //printf("ret: %d\n", ret);
    return 0;
}

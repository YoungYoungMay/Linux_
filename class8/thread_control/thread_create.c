//线程的创建
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_run(void* arg)//传入线程创建的函数
{
    while(1)
    {
        printf("I am thread 1\n");
        sleep(2);
    } 
}

int main()
{
    pthread_t tid;//无符号长整型
    //新线程去执行thread_run函数
    int ret = pthread_create(&tid, NULL, thread_run, "thread 1");//这里给thread_run传入的参数为一个字符串
    
    //这之后的是主线程在执行
    while(1)
    {
        printf("I am main thread\n");
        sleep(1);
    }
    return 0;
}

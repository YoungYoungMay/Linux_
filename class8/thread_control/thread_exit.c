//线程的终止
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void* thread_run(void* arg)//传入线程创建的函数
{
    //pthread_self函数用于获得当前线程id
    printf("I am %s, id is %lu\n", (char*)arg, pthread_self());
    //pthread_cancel(pthread_self());//线程取消自己
    //sleep(3);
    //pthread_exit((void*)99);
    //exit(123);//exit用于进程退出
}

int main()
{
    pthread_t tid;//无符号长整型
    //新线程去执行thread_run函数
    int ret = pthread_create(&tid, NULL, thread_run, "thread 1");//这里给thread_run传入的参数为一个字符串
    //这之后的是主线程在执行
    
    //sleep(2);
    pthread_cancel(tid);//取消线程
    
    void* ptr;
    //这里阻塞式等待,获得线程退出信息
    pthread_join(tid, &ptr);
    printf("main thread run, new thread ret: %d\n", (int)ptr);
    return 12;
}

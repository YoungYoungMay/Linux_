//所有线程的pid都是进程的pid，均相同
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

void* thread_run(void* arg)//传入线程创建的函数
{
    //pid_t tid = syscall(SYS_gettid);
    const char* msg = (const char*)arg;
    //printf("%s: %d\n", msg, tid);
    //这里打印出tid是地址的形式,是用户级的线程id,只在当前库内有效
    printf("%s: tid:%#x, pid:%d\n", msg, pthread_self(), getpid());
    sleep(5);

    pthread_exit((void*)0);
}

int main()
{
    pthread_t tid1, tid2, tid3;//无符号长整型
    pthread_create(&tid1, NULL, thread_run, "thread 1");
    pthread_create(&tid2, NULL, thread_run, "thread 2");
    pthread_create(&tid3, NULL, thread_run, "thread 3");
    
    pthread_join(tid1, NULL);//不关心线程退出信息
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    
    printf("main thread: tid:%x, pid:%d\n", pthread_self(), getpid());
    sleep(5);
    return 0;
}

#include "comm.c"

int main()
{
    int semid = CreateSem(1);//申请信号量为1的信号量集
    InitSem(semid, 0 , 1);//将信号量的计数值初始化为1

    pid_t id = fork();
    if(id == 0)//child
    {
        int _semid = GetSem(0);
        while(1)
        {
            P(_semid, 0);
            printf("A");
            fflush(stdout);
            usleep(123456);
            printf("A ");
            fflush(stdout);
            usleep(321456);
            V(_semid, 0);
        }
    }
    else//father
    {
        while(1)
        {
            P(semid, 0);
            printf("B");
            fflush(stdout);
            usleep(223456);
            printf("B ");
            fflush(stdout);
            usleep(121456);
            V(semid, 0);
        }
        wait(NULL);//不关心子进程的退出状态
    }
    DestroySem(semid);
    return 0;
}

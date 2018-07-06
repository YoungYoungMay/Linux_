#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h> //exit
#include <stdlib.h>

int main()
{
    pid_t id = fork();
    if(id == 0)//child
    {
        int count = 5;
        while(count--)
        {
            printf("heelo %d,pid is %d\n",count,getpid());
            sleep(1);
        }
        printf("child is quit\n");
        exit(0);
    }
    else if(id > 0)//father
    {
       //int st;//用以接受子进程的退出信息
       printf("pid:%d ,ppid: %d\n",getpid(),getppid());
       sleep(10);
       pid_t ret = wait(NULL);//此时不关心子进程的退出原因
       //pid_t ret = wait(&st);
       printf("father quit\n");
       sleep(1);
       printf("ret : %d\n",ret);//返回子进程pid或者-1(调用失败)
       //if(ret>0 && (st&0X7F)==0)//正常退出(status参数的低8位未收到任何信号)
       //{
       //     printf("child exit code:%d\n",(st>>8)&0XFF);
       //}
       //else if(ret > 0)
       //{
       //     printf("sig code:%d\n",st&0X7F);
       //}
    }
    else
    {
        perror("fork");
        exit(1);
    }
    return 0;
}

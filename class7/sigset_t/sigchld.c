#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void handler(int num)//信号处理函数
{
    pid_t id;
    //这里如果不用轮询式访问，进程可能会一直阻塞在当前函数
    while((id = waitpid(-1, NULL ,WNOHANG)) > 0)//确保所有子进程都被回收
    {
        printf("wait child success: %d\n",id);
    }
    printf("child is quit! pid %d\n",getpid());
}

int main()
{
    pid_t id = fork();
    //signal(SIGCHLD,handler);
    struct sigaction act,oact;
    act.sa_handler = SIG_IGN;
    act.sa_flags = 0;
    sigaction(SIGCHLD,&act,&oact);
    if(id < 0)
    {
        perror("fork");
        return -1;
    }
    else if(id == 0)//child
    {
        printf("child: pid is %d\n",getpid());
        sleep(3);
        exit(1);
    }
    else//father
    {
        while(1)
        {
            printf(" I am father\n");
            sleep(1);
        }
    }
    return 0;
}

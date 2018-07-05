//实现僵尸进程
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t id = fork();
    if(id < 0)//创建子进程失败
    {
        perror("fork");
        return 1;
    }
    else if(id > 0)//父进程
    {
        printf("parent[%d] is sleeping...\n",getpid());
        sleep(30);
    }
    else//子进程
    {
        printf("child[%d] is begin Z...\n",getpid());
        sleep(5);
        exit(1);
    }
    return 0;
}

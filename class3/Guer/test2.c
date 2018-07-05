//实现孤儿进程
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t id = fork();
    if(id < 0)
    {
        perror("fork");
        return 1;
    }
    else if(id == 0)
    {
        printf("I am a child,pid: %d\n",getpid());
        sleep(10);
    }
    else
    {
        printf("I am a parent,pid: %d\n",getpid());
        sleep(3);
        exit(0);
    }
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    printf("before: pid is %d\n",getpid());
    if((pid = fork()) == -1)//fork()子进程返回0，父进程返回子进程pid
    {
        perror("fork");
        exit(1);
    }
    printf("after:pid is %d,fork return is %d\n",getpid(),pid);
    sleep(1);
    return 0;
}

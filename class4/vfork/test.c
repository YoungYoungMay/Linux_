#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int flag = 100;
    pid_t pid = fork();
    //pid_t pid = vfork();
    if(pid == -1)
    {
        perror("fork()");
        //perror("vfork()");
        exit(1);
    }
    if(pid == 0)//child
    {
        flag = 200;
        printf("child flag is %d\n",flag);
        exit(0);
    }
    else//parent
    {
        sleep(1);
        printf("parent flag is %d\n",flag);
    }
    return 0;
}

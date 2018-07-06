#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int status = 0;
    status = system("ls -l");
    if(status == -1)
    {
        perror("system");
        exit(1);
    }
    if(WIFEXITED(status) != 0)//正常退出
    {
        if(WEXITSTATUS(status) == 0)//操作正确
        {
            printf("run success\n");
        }
        else
        {
            printf("run failed,exit code is %d\n",WEXITSTATUS(status));
        }
    }
    else//异常退出
    {
        printf("sig code is %d\n",WEXITSTATUS(status));
    }
}

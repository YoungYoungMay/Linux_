//raise函数向调用它的进程发送信号
#include <stdio.h>
#include <signal.h>//raise
#include <stdlib.h>//atoi
#include <unistd.h>//sleep

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("可执行程序+signo\n");
        return -1;
    }
    int i = 10;
    while(i--)
    {
        printf("This is YoungMay\n");
        sleep(1);
        if(i == 5)
        {
            int ret = raise(atoi(argv[1]));
            if(ret != 0)//调用成功返回0，否则返回非0
            {
                perror("raise");
                return -2;
            }
            else
            {
                printf("send %d succeed\n",atoi(argv[1]));
            }
        }
    }
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char* argv[])//利用命令行参数向另一进程发送信号
{
    if(argc != 3)//命令格式输入错误
    {
        printf("可执行文件+pid+signo\n");
        return -1;
    }
    //atoi函数用于将字符串转换为int型
    int ret = kill(atoi(argv[1]),atoi(argv[2]));//调用kill向进程发信号
    if(ret == -1)
    {
        perror("kill");
        return -2;
    }
    else
    {
        printf("send %d to %d succeed\n",atoi(argv[2]),atoi(argv[1]));
    }
    return 0;
}

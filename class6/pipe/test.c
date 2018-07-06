#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd[2];
    int ret = pipe(fd);
    if(ret != 0)//创建管道成功返回0
    {
        perror("pipe");
        return;
    }
    pid_t id = fork();
    if(id < 0)
    {
        perror("fork");
        return;
    }
    else if(id == 0)//child-w
    {
        close(fd[0]);//关闭读端
        const char* msg = "hello, I am YoungMay\n";
        while(1)
        {
            write(fd[1],msg,strlen(msg));//向管道写数据
            sleep(1);
        }
    }
    else//father->r
    {
        close(fd[1]);//关闭写端
        char buf[64];//用于存放管道中读到的数据
        while(1)
        {
            size_t s = read(fd[0],buf,sizeof(buf));//返回实际读到的字节数
            if(s > 0)//读成功
            {
                buf[s] = '\0';
                printf("%s",buf);
            }
        }
    }
    return 0;
}

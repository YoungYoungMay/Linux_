//将txt文件内容写入管道
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    mkfifo("myfifo", 0644);
    //得到txt文件的文件描述符
    int infd = open("txt",O_RDONLY);//成功返回文件描述符，失败返回-1
    if(infd == -1)
    {
        perror("open");
        return;
    }
    //得到管道文件的文件描述符
    int outfd = open("myfifo",O_WRONLY);
    if(outfd == -1)
    {
        perror("open");
        return;
    }

    char buf[64];
    size_t s;
    while((s = read(infd,buf,sizeof(buf))) > 0)//将txt文件内容读到buf
    {
        write(outfd,buf,s);//将txt读到buf中的内容写到管道中
    }
    close(infd);
    close(outfd);
    return 0;
}

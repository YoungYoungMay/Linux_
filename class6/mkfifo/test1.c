//将管道内容写入文件txt1
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    mkfifo("myfifo",644);
    //得到输出文件的文件描述符
    int outfd = open("txt1",O_WRONLY | O_CREAT | O_TRUNC,0644);//成功返回文件描述符，失败返回-1
    //O_TRUNC表示文件若已存在，且以只写或可读可写方式打开，则将其长度截断为0字节
    if(outfd == -1)
    {
        perror("open");
        return;
    }
    //得到管道文件的文件描述符
    int infd = open("myfifo",O_RDONLY);
    if(infd == -1)
    {
        perror("open");
        return;
    }

    char buf[64];
    size_t s;
    while((s = read(infd,buf,sizeof(buf))) > 0)//将管道中数据读到buf
    {   
        write(outfd,buf,s);//将管道中读到buf中的内容写到txt1中
    }
    close(infd);
    close(outfd);
    unlink("myfifo");//删除指定名字的文件
    return 0;
}

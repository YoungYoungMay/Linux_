//接收client发来的内容
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int ret = mkfifo("myfifo", 0644);
    if(ret < 0)
    {
        perror("mkfifo");
        return 1;
    }
    int fd = open("./myfifo",O_RDONLY);//read
    if(fd < 0)
    {
        perror("open");
        return 2;
    }
    char buf[64];
    while(1)
    {
        buf[0] = 0;        
        size_t s = read(fd,buf,sizeof(buf)-1);//将管道内容读到buf中
        if(s > 0)
        {
            buf[s] = '\0';
            printf("server# %s\n",buf);
        }
        else if(s == 0)//读完了
        {
            printf("client quit\n");
            break;
        }
        else
        {   
            perror("read");
            return 3;
        }
    }
    close(fd);
    return 0;
}

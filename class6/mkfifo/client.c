//向server发消息
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

int main()
{
    int fd = open("./myfifo",O_WRONLY);//write
    if(fd < 0)
    {
        perror("open");
        return 1;
    }
    char buf[64];
    while(1)
    {
        buf[0] = 0;
        printf("Please Enter:");
        fflush(stdout);
        //scanf("%s",buf);//输入空格时会有问题
        size_t s = read(0,buf,sizeof(buf)-1);//从键盘读数据
        if(s > 0)
        {
            buf[s] = 0;
          write(fd,buf,strlen(buf));//向管道内写数据,向文件里写不用读入'\0'
        }
        else
        {
            perror("read");
            return 2;
        }
    }
    close(fd);
    //unlink("myfifo");//删除myfifo命名管道
    return 0;
}

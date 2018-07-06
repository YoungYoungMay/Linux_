//alarm函数会给进程发送SIGALRM信号
#include <stdio.h>
#include <unistd.h>

int main()
{
    unsigned int ret = alarm(2);//两秒之后响的闹钟
    printf("This is YoungMay\n");
    sleep(1);
    ret = alarm(0);
    printf("ret :%d\n",ret);//取消闹钟
    //sleep(3);
    printf("That is fafa\n");
    return 0;
}

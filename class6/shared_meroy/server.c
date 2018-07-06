#include "comm.c"
#include<unistd.h>

int main()
{   
    int shmid = CreateShm(4096);//创建一个共享内存
    char* addr = shmat(shmid, NULL, 0);//将共享内存段连接到进程的地址空间,成功返回指向共享内存的第一个字节
    sleep(6);//为了能够等到client进程向共享内存放消息，具体等几秒自己把握，如不等，接收的消息会少一点
    int i = 0;
    while(i++ < 26)
    {
        printf("client# %s\n",addr);
        sleep(1);//这里不等的话，26次循环很快就执行完了，你还没等到client向共享内存里放消息，它就运行完了
    }
    shmdt(addr);//将共享内存段与当前进程脱离
    sleep(2);
    DestroyShm(shmid);
    return 0;
}

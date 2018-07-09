#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
    
void mydaemon()
{
    //1.将文件模式创建屏蔽字设置为0，保证在每个平台守护进程都可以提供服务
    umask(0);
    //2.调用fork，让父进程退出，保证子进程不是进程组的组长进程
    //如果该守护进程是作为一条简单的shell命令启动的，那么父进程终止使得shell认为该命令已经执行完了
    pid_t id = fork();
    if(id < 0)//创建失败
    {
        perror("fork");
        exit(1);
    }
    else if(id > 0)//father
    {
        exit(0);//终止父进程
    }

    //3.调用setsid创建一个新会话
    if(setsid() < 0)//创建失败
    {
        perror("setsid");
        exit(2);
    }
    
    //4.因为新成会话，新成进程组的子进程可能会再创建子进程
    //子进程退出若不回收资源，可能会内存泄漏，但是回收的话又会造成守护进程的负担
    //所以我们将子进程退出时的发送的SIGCHLD信号设置为SIG_IGN(忽略)
    //这样子进程创建出来的子进程在终止时会自动清理，不会产生僵尸进程，也不会通知父进程
    signal(SIGCHLD, SIG_IGN);
  
    //(这里可能会)再次fork,终止父进程，保持子进程不是话首进程，从而保证后续不会再和其他终端关联
    //这部分不是必须的



    //5.将工作目录改为根目录
    chdir("/");

    //6.关闭不需要的文件描述符
    //守护进程不用stdin、stdout、stderror,错误以日志文件呈现
    close(0);
    close(1);
    close(2);

   
}

int main()
{
    //mydaemon();//调用成为守护进程
    daemon(0, 0);

    while(1)
    {
        //do some service
        sleep(1);
    }
    return 0;
}

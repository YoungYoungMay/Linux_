//实现sleep函数
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int num)
{
    ;
}

unsigned int mysleep(unsigned int t)
{
    struct sigaction act,oact;//act为要设置闹钟信号的相关信息，oact保存闹钟信号的原有相关信息
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);//处理闹钟信号时，不屏蔽其他信号
    sigaction(SIGALRM,&act,&oact);//捕捉闹钟信号
    
    alarm(t);//t秒之后向进程发送闹钟信号
    pause();//使进程挂起等待
    int ret = alarm(0);//取消闹钟,返回闹钟剩余秒数
    sigaction(SIGALRM,&oact,NULL);//恢复闹钟的默认处理动作
    return ret;//返回闹钟剩下的时间
}

int main()
{
    while(1)
    {
        printf("hi,i am youngmay\n");
        mysleep(1);
    }
    return 0;
}

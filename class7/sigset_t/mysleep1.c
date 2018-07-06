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
    sigset_t new,old;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);//处理闹钟信号时，不屏蔽其他信号
    sigaction(SIGALRM,&act,&oact);//捕捉闹钟信号
    
    sigemptyset(&new);//初始化信号集
    sigaddset(&new,SIGALRM);//将闹钟信号在信号集内设为有效信号
    sigprocmask(SIG_BLOCK,&new,&old);//屏蔽闹钟信号
    
    alarm(t);//t秒之后向进程发送闹钟信号
    
    sigdelset(&new,SIGALRM);//将信号集中闹钟信号置为无效
    sigsuspend(&new);//解除对闹钟信号的屏蔽，并使进程挂起等待
    unsigned int ret = alarm(0);//取消闹钟,返回闹钟剩余秒数
    sigaction(SIGALRM,&oact,NULL);//恢复闹钟的默认处理动作
    sigprocmask(SIG_UNBLOCK,&old,NULL);//取消对闹钟信号的屏蔽
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

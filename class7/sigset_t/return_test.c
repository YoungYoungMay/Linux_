//测试mysleep函数的返回值是否是我们预期的闹钟的剩余值
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int num)
{
    printf("signo is %d\n",num);
    return;
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
    signal(2,handler);//将2号信号的处理动作改为自定义函数
    printf("hello\n"); 
    unsigned int ret = mysleep(20);
    printf("return ret is %d\n",ret);
    return 0;
}

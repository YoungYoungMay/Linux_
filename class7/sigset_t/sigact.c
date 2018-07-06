#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int num)
{
    printf("signo is %d\n",num);
    return;
}

int main()
{
    struct sigaction act,oact;
    act.sa_handler = handler;//将信号处理动作设置为信号捕捉
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,3);//屏蔽3号信号
    sigaction(2,&act,&oact);//自定义2号信号的处理动作
    sigaction(3,&act,&oact);//自定义3号信号的处理动作
    int i = 10;
    while(i--)
    {
        printf("this is youngmay\n");
        if(i == 5)
        {
            sigaction(2,&oact,NULL);//恢复2号信号的默认处理动作
            oact.sa_handler = SIG_IGN;//将3号信号的处理动作设为忽略
            sigaction(3,&oact,NULL);
        }
        sleep(1);
    }
    return 0;
}

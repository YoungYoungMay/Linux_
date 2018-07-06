#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void printsigset(sigset_t* set)//打印pending表
{
    int i = 0;
    for(i=1; i<=32; ++i)//信号编号从0开始
    {
        if(sigismember(set,i))//当前信号在信号集中
            putchar('1');
        else//当前信号不在信号集中
            putchar('0');
    }
    puts("");
    //printf("\n");
}

int main()
{
    sigset_t s,p;
    sigemptyset(&s);//初始化信号集
    sigaddset(&s,2);//将2号信号设置为有效信号
    sigprocmask(SIG_BLOCK, &s, NULL);//屏蔽2号信号
    int i = 10;
    while(i--)
    {
        sigpending(&p);//获取当前进程的未决信号集
        printsigset(&p);//打印未决信号集
        if(i==7)
        {
            raise(2);//向本进程发送2号信号
        }
        if(i == 5)
        {
            sigprocmask(SIG_UNBLOCK, &s, NULL);//解除对2号信号的屏蔽
            printf("recober block bitmap\n");
        }
        sleep(1);
    }
    return 0;
}

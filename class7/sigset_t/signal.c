#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int num)//捕捉信号
{
    printf("signo is %d\n",num);
    return;
}

int main()
{
    signal(2,handler);//捕捉2号信号
    signal(3,handler);//捕捉3号信号

    while(1)
    {
        printf("this is youngmay\n");
        sleep(1);
    }
    return 0;
}

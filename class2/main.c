//实现一个彩色进度条
#include <stdio.h>
#include <unistd.h>

#define END "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define PORPLE "\033[35m"
#define DGREEN "\033[36m"
#define WHITE "\033[37m"

int main()
{
	int i = 0;
    char buf[120] = "#";
    for(i=1; i<=100; i++)
	{
		buf[i] = '#';
        //\r把光标定义在下一行的开头位置
        int c = i%8;
        switch(c)
        {
            case 0:
                printf(BLACK);
                break;
            case 1:
                printf(RED);
                break;
            case 2:
                printf(GREEN);
                break;
            case 3:
                printf(YELLOW);
                break;
            case 4:
                printf(BLUE);
                break;
            case 5:
                printf(DGREEN);
                break;
            case 6:
                printf(PORPLE);
                break;
            case 7:
                printf(WHITE);
                break;

        }
        //这里%-100s占位100个字符，不够用空格布满
        //不加-就从左补空格，右边打印；加-从右补空格，左边打字符
        printf("[%-100s][%d%]\r", buf, i);
		printf(END);
        fflush(stdout);
        //这里稍停0.1秒
		usleep(100000);//单位为微秒，sleep函数单位为秒
	}
	printf("\n");
}	

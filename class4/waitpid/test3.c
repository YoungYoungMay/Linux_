//利用status的两个宏
#include <stdio.h>
#include <unistd.h>//fork
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t id = fork();
    if(id < 0)
    {
        perror("fork");
        return 1;
    }
    else if(id == 0)//child
    {
        int count = 5;
        while(count--)
        {
            printf("child : pid is %d, ppid is %d\n",getpid(),getppid());
            sleep(5);
        }
        printf("child is quit\n");
        exit(13);
    }
    else//parent
    {
        int status = 0;
        printf("father: pid %d, ppid %d\n",getpid(),getppid());
        //while(1)//循环实现轮询
        //{
        //    pid_t ret = waitpid(id,&status,WNOHANG);//非阻塞访问
        //    if(ret > 0)//成功等到子进程
        //    {
        //        printf("sig :%d, exit code: %d\n",status&0X7F,(status>>8)&0XFF);//打印退出信息(异常退出的信号、正常退出码)
        //        break;
        //    }
        //    else if(ret == 0)//子进程还未退出,轮询访问
        //    {
        //        printf("child is running,do other things\n");
        //    }
        //    else//等待失败
        //    {
        //        break;
        //    }
        //    sleep(1);
        //}
        //printf("father quit\n");
        //sleep(1);
        //printf("ret : %d\n",ret);//输出子进程pid
        pid_t ret= 0;
        do
        {
            ret = waitpid(id,&status,WNOHANG);
            if(ret == 0)
            {
                printf("child is running,do other things\n");
            }
            sleep(1);
        }while(ret == 0);//轮询访问，直至子进程退出
        
        if(WIFEXITED(status) && ret == id)//进程正常退出，且等到的是指定子进程退出
        {
            printf("exit code is %d\n",WEXITSTATUS(status));//WEXITSTATUS(status))查看进程的退出码
        }
        else if(!(WIFEXITED(status)))
        {
            printf("sig code :%d\n",status & 0X7F);
        }
        printf("father quit\n");
        //sleep(1);
    }
    return 0;
}

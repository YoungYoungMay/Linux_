//非阻塞式等待
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
            sleep(1);
        }
        printf("child is quit\n");
        exit(13);
    }
    else//parent
    {
        int status = 0;
        printf("father: pid %d, ppid %d\n",getpid(),getppid());
        //sleep(10);
        while(1)
        {
            pid_t ret = waitpid(id,&status,WNOHANG);//非阻塞访问
            if(ret > 0)//成功等到子进程
            {
                printf("sig :%d, exit code: %d\n",status&0X7F,(status>>8)&0XFF);//打印退出信息(异常退出的信号、正常退出码)
                break;
            }
            else if(ret == 0)//子进程还未退出,轮询访问
            {
                printf("child is running,do other things\n");
            }
            else//等待失败
            {
                break;
            }
            sleep(1);
        }
        printf("father quit\n");
        sleep(1);
        //printf("ret : %d\n",ret);//输出子进程pid
        //if(WIFEXITED(status) && ret == id)//WIFEXITED(status)为真表示进程正常退出，且等到的是指定子进程退出
        //{
        //    printf("wait success,child return code is %d\n",WEXITSTATUS(status));//WEXITSTATUS(status))查看进程的退出码
        //}
        //else
        //{
        //    printf("wait child failed,return \n");
        //    return 2;
        //}
    }
    return 0;
}

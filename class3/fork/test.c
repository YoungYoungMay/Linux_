#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

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
    //    while(1)
      //  {
            printf("do my thing1...pid:%d,ppid:%d\n",getpid(),getppid());
        //    sleep(1);
        //}
    }
    else//parent
    {   
        printf("do my thing2...pid:%d,ppid:%d\n",getpid(),getppid());
        //sleep(3);
    }
    sleep(1);
    return 0;
}

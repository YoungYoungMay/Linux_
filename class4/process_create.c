#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void process_create(int(*func)(), const char* file, char* argv[])
{
    int ret = 0;
    pid_t pid = fork();
    if(pid == 0)//child
    {
        ret = func(file,argv);
        if(ret == -1)
        {
            perror("func");
            exit(1);
        }
    }
    else
    {
        int st;
        pid_t ret = wait(&st);
        if(ret == -1)
        {
            perror("wait");
            exit(2);
        }
    }
}

int main()
{
    char* argv1[] = {"ls"};
    char* argv2[] = {"ls","-l","/home/may/Code/Linux/class4",0};
    process_create(execvp,*argv1,argv2);
    return 0;
}

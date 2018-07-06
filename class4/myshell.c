#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>  //isspace

int main()
{
    char buf[1024] = {};
    while(1)
    {
        pid_t id = fork();
        if(id == 0)//child
        {
            char* my_arg[32];
            printf("myshell>");
            fgets(buf,sizeof(buf),stdin);
            buf[strlen(buf)-1] = 0;
            char* p = buf;
            int i = 1;
            my_arg[0] = buf;
            while(*p)
            {
                if(isspace(*p))
                {
                    *p = 0;
                    p++;
                    my_arg[i++] = p;
                }
                else
                {
                    p++;
                }
            }
            my_arg[i] = NULL;
            if(strcmp(*my_arg,"exit") == 0)
            {
                _exit(1);
            }
            execvp(my_arg[0],my_arg);
        }
        else
        {
            int status = 0;
            pid_t ret = waitpid(id,&status,0);
            if(ret > 0)
            {
                //printf("%d\n",status&0X7F);
            }
            else
            {
                printf("waitpid runnning error\n");
            }
        }
    }
    return 0;
}

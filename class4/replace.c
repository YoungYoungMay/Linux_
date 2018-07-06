#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    char* const argv[] = {"ls","-l","NULL"} ;
    char* const envp[] = {"PATH=/bin:/usr/bin","TERM=console",NULL};
    //execl("/bin/ls","ls","-l",NULL); //带l的表示参数采用列表，以NULL结尾
    //execlp("ls","ls","-l",NULL);//带p自动搜索环境变量PATH
    //execle("ls","ls","-l",NULL,envp);//表示自己维护环境变量
    execv("/bin/ls",argv);//带v的参数用数组
    exit(0);
}

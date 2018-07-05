//验证环境变量通常具有全局性，同时使用export命令
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char* env =getenv("MYENV");
    export MYENV = "hello world";
    if(env)
    {
        printf("%s\n",env);
    }
    return 0;
}

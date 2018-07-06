#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    printf("pid: %d\n",getpid());
    while(1);
    return 0;
}

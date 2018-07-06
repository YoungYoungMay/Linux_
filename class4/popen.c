#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    FILE* file = NULL;
    char buf[1024] = {0};
    file = popen("cat replace.c","r");
    if(file == NULL)
    {
        perror("popen");
        exit(1);
    }
    while(fgets(buf,1024,file) != NULL)
    {
        fprintf(stdout,"%s",buf);
    }
    pclose(file);
    return 0;
}

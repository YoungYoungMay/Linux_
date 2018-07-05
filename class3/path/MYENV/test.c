#include <stdio.h>
#include <stdlib.h>

int main()
{
    char* env = getenv("MY_VAL");
    if(env)
    {
        printf("%s\n",env);
    }
    return 0;
}

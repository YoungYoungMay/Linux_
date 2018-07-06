#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void* run(void* arg)
{
    int i;
    for( ; ; )
    {
        printf("i am thread 1\n");
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    int ret;
    if((pthread_create(&tid, NULL, run, NULL)) != 0)
    {
        fprintf(stderr, "pthread_create : %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    
    int i;
    for( ; ; )
    {
        printf("I am main thread\n");
        sleep(1);
    }
    return 0;
}

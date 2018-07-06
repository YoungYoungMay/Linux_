#include "comm.c"
#include<unistd.h>

int main()
{
    int shmid = GetShm(4096);//获得一个共享内存的标识码
    sleep(1);
    char* addr = shmat(shmid, NULL, 0);
    sleep(2);
    char x = 'A';
    for(x='A'; x<='Z'; x++)//依次发送字母A-Z
    {
        addr[x-'A'] = x;
        addr[x-'A'+1] = '\0';
        sleep(1);
    }
    shmdt(addr);
    sleep(2);
    return 0;
}

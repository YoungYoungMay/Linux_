#include "comm.h"

static int _CreateShm(int size, int flags)
{
    key_t key = ftok(PATHNAME, PROJ_ID);
    if(key < 0)
    {
        perror("ftok");
        return -1;
    }
    int shmid;//共享内存标识码(非负整数)
    if((shmid = shmget(key, size, flags)) < 0)//创建共享内存失败时返回-1
    {
        perror("shmget");
        return -2;
    }
    return shmid;//将创建的共享内存的标识码返回，以实现后续代码
}

int CreateShm(int size)//创建一个新的共享内存
{
    return _CreateShm(size, IPC_CREAT | IPC_EXCL | 0666);//已有该共享内存，返回-1；否则创建再返回标识码
}

int GetShm(int size)//获得共享内存标识码
{
    return _CreateShm(size, IPC_CREAT);//已有该共享内存，返回标识码即可；没有则创建则创建
}

int DestroyShm(int shmid)
{
    if((shmctl(shmid, IPC_RMID, NULL)) < 0)
    {
        perror("shmctl");
        return -1;
    }
    return 0;
}

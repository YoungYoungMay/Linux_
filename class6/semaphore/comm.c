//实现信号量的相关操作的函数
#include "comm.h"

static int _CreateSem(int nsems, int flags)//创建一个信号量
{
    key_t key = ftok(PATHNAME, PROJ_ID);//产生key值
    if(key < 0)
    {
        perror("ftok");
        return -1;
    }
    int semid = semget(key, nsems, flags);//创建一个信号量
    if(semid < 0)
    {
        perror("semget");
        return -2;
    }
    return semid;
}

int CreateSem(int nsems)//获得一个新的信号量集
{
    return _CreateSem(nsems, IPC_CREAT | IPC_EXCL | 0666);
}

int GetSem(int nsems)//获得一个信号量集的标识码
{
    return _CreateSem(nsems, IPC_CREAT);
}

int InitSem(int semid, int semnum, int initval)//对信号量集进行初始化
{
    union semun _un;
    _un.val = initval;
    if(semctl(semid, semnum, SETVAL, _un ) < 0)//设置信号量集中信号量的计数值
    {
        perror("semctl");
        return -1;
    }
    return 0;
}

static int SemPV(int semid, int who, int op)//PV操作实现
{
    struct sembuf _sf;
    _sf.sem_num = who;//通过信号量的编号确定对哪个信号量进行操作
    _sf.sem_op = op;//信号量一次PV操作时加减的数值
    _sf.sem_flg = 0;
    if(semop(semid, &_sf, 1) < 0)
    {
        perror("semop");
        return -1;
    }
    return 0;
}

int P(int semid, int who)//对信号量进行P操作
{
    return SemPV(semid, who, -1);//减1操作
}

int V(int semid, int who)//对信号量进行V操作
{
    return SemPV(semid, who, +1);//即加1操作
}

int DestroySem(int semid)//销毁信号量集
{
    if(semctl(semid, 0, IPC_RMID) < 0)//删除信号量集中序号为0的信号量
    {
        perror("semctl");
        return -1;
    }
}

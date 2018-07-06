#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <wait.h>
#include <unistd.h>

#define PATHNAME "."
#define PROJ_ID 0x6666

union semun
{
    int val;//SETVAL用的值
    struct semid_ds* buf;//IPC_STAT、IPC_SET用的
    unsigned short* array;//GETALL、SETALL用的数组值
    struct seminfo* _buf;//为IPC_INFO提供的缓存
};

#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define PATHNAME "."
#define PROJ_ID 0x6666

//接收消息的优先级
#define SERVER_TYPE 1
#define CLIENT_TYPE 2

struct msgbuf//一个消息结构
{
    long mtype;
    char mtext[1024];
};

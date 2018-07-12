#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#define MAXSIZE 128

typedef struct
{//客服端发送的用于计算的两个数据
    int x;
    int y;
}request;

typedef struct
{//服务器端计算完返回给客户端的值
    int res;
}response;

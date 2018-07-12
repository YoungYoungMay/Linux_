//实现网络版加法计算器，利用TCP多线程服务器计算客户端发过来数的和
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "comm.h"

typedef struct
{
    int sock;
    char ip[24];
    int port;
}net_info_t;

int startup(int port, char* ip)
{
    //1.创建套接字,这里是流式的套接字，因为TCP面向字节流
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        printf("socket error\n");
        exit(2);//套接字创建失败，直接终止进程,因为没有套接字网络通信根本无法实现，后续代码根本不用执行
    }
    //2.绑定
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(port);//端口号
    local.sin_addr.s_addr = inet_addr(ip);//IP

    if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
        printf("bind error\n");
        exit(3);
    }

    //3.监听:一直等待客户来连接(因为TCP是面向连接的)，提取新连接；
    if(listen(sock, 5) < 0)//最多容纳5个客户连接请求
    {
        printf("listen error\n");
        exit(4);
    }
    return sock;//返回一个监听套接字
}

void service(int sock, char* ip, int port)
{
    while(1)
    {
        request r;
        //read函数可以读取任意类型的内容
        ssize_t s = read(sock, &r, sizeof(r));
        if(s < 0)
        {
            perror("read");
            exit(5);
        }
        else if(s == 0)
        {
            printf("client[%s|%d] quit\n", ip, port);
            printf("####################################\n");
            
            return;
        }
        else
        {
            response rp;
            rp.res = r.x + r.y;
            write(sock, &rp, sizeof(rp));
        }
    }
}

void* thread_service(void* arg)
{
    net_info_t *p = (net_info_t* )arg;
    service(p->sock, p->ip, p->port);

    close(p->sock);
    free(p);
}

//./tcp_server 127.0.0.1 8080
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage: %s [ip] [port]\n", argv[0]);
        exit(1);
    }
    
    //创建套接字
    int listen_sock = startup(atoi(argv[2]), argv[1]);
    
    struct sockaddr_in peer;
    char ipbuf[24];
    for( ; ; )
    {
        ipbuf[0] = 0;
        socklen_t len = sizeof(peer);
        //从监听套接字中拿连接
        int new_sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
        if(new_sock < 0)//拿连接失败，不用管，因为还可以去拿其他连接
        {
            printf("accept error\n");
            continue;
        }
        //获得了一个新连接
        inet_ntop(AF_INET, (const void*)&peer.sin_addr, ipbuf, sizeof(ipbuf));//将四字节IP地址转换为点分十进制
        int p = ntohs(peer.sin_port);//端口号：网络序列转换为主机序列的端口号
        printf("get a new connect,[%s:%d]\n", ipbuf, p);//将新连接的IP和端口号打印
        printf("####################################\n");

        //这里不用像多进程的版本关闭多余的文件描述符
        //因为线程共享地址空间，关掉一个文件，其他线程就看不到用不了了
        net_info_t* info = (net_info_t*)malloc(sizeof(net_info_t));
        if(info == NULL)
        {
            perror("malloc");
            close(new_sock);
            continue;
        }
        info->sock = new_sock;
        strcpy(info->ip,ipbuf);
        info->port = p;

        pthread_t tid;
        pthread_create(&tid, NULL, thread_service, (void* )info);
        pthread_detach(tid);//线程分离后，该线程的资源会自动释放
    }
    return 0;
}

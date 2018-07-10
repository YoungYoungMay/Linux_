//TCP客户端
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

#define MAXSIZE 128

//./tcp_client 127.0.0.1 8080
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage: %s [ip] [port]\n", argv[0]);
        return 1;
    }
    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        printf("socket error\n");
        return 2;
    }
    //客户端不用绑定(bind)，不用监听(listen)，不用获取新连接(accept)
    
    //客户端有一个个性化操作connect,向服务器发起连接
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);
    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        printf("connect error\n");
        return 3;
    }
    //走到这，连接成功

    char buf[MAXSIZE];
    while(1)
    {
        printf("please enter# ");
        fflush(stdout);
        ssize_t s = read(0, buf, sizeof(buf)-1);
        if(s > 0)
        {
            buf[s-1] = 0;//去掉回车符号
            if(strcmp("quit", buf) == 0)//客户端输入quit表示客户端退出
            {
                printf("client quit\n");
                break;
            }
            write(sock, buf, strlen(buf));
            s = read(sock, buf, sizeof(buf)-1);
            buf[s] = 0;
            printf("serevr echo# %s\n",buf);

        }

    }
    close(sock);
    return 0;
}

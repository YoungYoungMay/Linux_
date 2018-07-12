//HTTP服务器，基于TCP协议
//简单版本:可以收到客户端的请求信息
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>//bzero清空
#include <string.h>
#include <sys/wait.h>

void service(int sock)
{//对客户端的请求不做任何分析，均响应回去一个"hello,..."
    char buf[10240];
    ssize_t s = read(sock, buf, sizeof(buf));
    if(s < 0)
    {
        perror("read");
        exit(5);
    }
    if(s > 0)
    {
        printf("%s", buf);
    }
    //HTTP响应：给一个响应行、一个空行,正文，不给响应报头
    //HTTP的1.0版本是基于短链接的
    const char* response = "HTTP/1.0 200 OK\n\n<html><head>hello, this is YoungMay</head></html>\n";
    write(sock, response, strlen(response));
}

int startup(int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        printf("socket error\n");
        exit(2);
    }

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    //参数 宏INADDR_ANY，可以监听主机所有IP
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
        printf("bind error\n");
        exit(3);
    }

    if(listen(sock, 5) < 0)
    {
        printf("listen error\n");
        exit(4);
    }
    return sock;
}

//./myhttpd 8080
int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s [port]\n", argv[0]);
        return 1;
    }
    
    int listen_sork = startup(atoi(argv[1]));
    
    //daemon(0, 0);

    while(1)
    {
        struct sockaddr_in peer;
        bzero(&peer, sizeof(peer));
        socklen_t len = sizeof(peer);
        int sock = accept(listen_sork, (struct sockaddr*)&peer, &len);
        if(sock < 0)
        {
            //write log
            continue;
        }
        int id = fork();
        if(id == 0)//child
        {
            close(listen_sork);
            if(fork() > 0)
            {
                exit(0);
            }
            service(sock);
            close(sock);
            exit(0);
        }
        else if(id > 0)//father
        {
            close(sock);
            waitpid(id, NULL, 0);
        }
    }
    return 0;
}

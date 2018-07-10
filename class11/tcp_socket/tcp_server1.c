//TCP多进程服务器,支持多进程
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/wait.h>

#define MAXSIZE 128

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
    char buf[MAXSIZE];
    while(1)
    {
        //TCP是流式套接字，可直接用读写文件的接口操作
        //read返回值为0时，说明对方(客户端)已关闭
        ssize_t s = read(sock, buf, sizeof(buf)-1);
        if(s > 0)
        {
            buf[s] = 0;
            printf("[%s:%d]say# %s\n", ip, port, buf);
            //TCP套接字是全双工的,两个可以同时读写
            write(sock, buf, strlen(buf));
        }
        else if(s == 0)
        {
            printf("client [%s:%d] quit!\n", ip, port);
            break;
        }
        else
        {
            printf("read error\n");
            break;
        }
    }
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

        //父进程获取新连接，子进程为其提供服务，提供完子进程退出；然后父进程再获取，子进程再通过
        pid_t id = fork();
        if(id == 0)//child提供服务
        {
            //子进程继承父进程文件描述符表，打开了两个套接字：listen_sock、new_sock,
            //而子进程不需要listen_sock,防止意外，所以关闭它
            close(listen_sock);

            if(fork() > 0)//它退出后，它的子进程变成孤儿进程，被1号进程回收
            {
                exit(0);
            }
            //提供完服务，就关闭，然后子进程退出
            service(new_sock, ipbuf, p);
            close(new_sock);
            exit(0);
        }
        else if(id > 0)//father获得新连接
        {
            //上面子进程可以不关listen_scok,因为不去访问它即可，但是这里父进程必须关闭
            //因为每次创建子进程，都会给它文件描述符，这里不关，创建一个子进程给一个文件描述符，
            //但文件描述符有上限，所以关闭它
            close(new_sock);
            
            //父进程需要等待子进程
            //否则每次创建的子进程退出了，但是父进程不退出，循环下去会产生很多僵尸进程，可能导致内存泄漏问题
            //阻塞等待让父进程卡住，损耗效率;非阻塞等待也不好用，也有可能造成内存泄漏;信号章节的等待方式有局限性
            waitpid(id, NULL, 0);
        }
        else//创建失败，因为系统压力太大，不能承受新连接
        {
            printf("fork error\n");
            continue;
        }
    }
    return 0;
}

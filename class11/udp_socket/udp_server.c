//UDP服务器
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>//close
#include <arpa/inet.h>//网络字节序和主机字节序库转换
#include <stdlib.h>//atoi
#include <netinet/in.h>
#include <string.h>

#define SIZE 128

void service(int sock)
{//这里我们提供的服务是：将客服端的消息拿到并打印，再将消息发回去
    char buf[SIZE];
    for( ; ; )//服务器应该周期性的一直提供服务，可用守护进程实现，这里先用死循环实现
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);//socklen_t 可认为简单整形
        //udp不用read、write读写，它有自己个性化的接口
        ssize_t s = recvfrom(sock, buf, sizeof(buf)-1, 0 , (struct sockaddr*)&peer, &len);
        if(s > 0)//读成功
        {
            buf[s] = 0;//保证以\0结尾，上面参数sizeof-1就是给\0留位置
            if(strcmp("quit", buf) == 0)
            {
                printf("client quit\n");
                break;
            }
            printf("[%s:%d]# %s\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port), buf);//将客户端发来的消息打印出来
            sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&peer, len);//将内容给客户端发回去
        }
    }
}

//./udp_server 192.168.3.55 8080
int main(int argc, char* argv[])
{
    if(argc != 3)//命令行输入格式不正确
    {
        printf("Usage: %s [IP] [port]\n", argv[0]);
        return 3;
    }
    //1.创建套接字(这里是数据报的套接字，因为UDP面向数据报)
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)//创建套接字失败
    {
        printf("socket error\n");
        return 1;
    }
    
    struct sockaddr_in local;
    local.sin_family = AF_INET;//地址类型
    local.sin_port = htons(atoi(argv[2]));//端口号;命令行的字符串”808"转换为整数的8080;主机序列转换为网络序列，16位
    local.sin_addr.s_addr = inet_addr(argv[1]);//IP地址;将点分十进制转换为四字节地址(因为该字段在结构体中就是四字节);

    //2.绑定(将文件信息与网络信息关联起来),绑定之后就可提供服务了
    if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
        printf("bind error\n");
        return 2;
    }

    //3.提供服务
    service(sock);

    close(sock);//Linux下一切皆文件，能打开就要关闭
    return 0;
}

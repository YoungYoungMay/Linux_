//UDP客户端
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>//close
#include <arpa/inet.h>//网络字节序和主机字节序库转换
#include <stdlib.h>//atoi
#include <netinet/in.h>
#include <string.h>


//./udp_client 127.0.0.1 8080
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage:%s [ip] [port]\n", argv[0]);
        return 1;
    }
    //客户端不需要绑定，因为它的端口号不需要特定
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        printf("sock error\n");
        return 2;
    }

    char buf[128];
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);
    socklen_t len = sizeof(server);

    for( ; ; )
    {
        buf[0] = 0;//清零
        //1.从标准输入读数据
        ssize_t s = read(0, buf, sizeof(buf)-1);
        if(s > 0)//读成功
        {
            buf[s-1] = 0;//会把回车读进来，这里不要回车
            //2.将读到的数据发出去
            sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&server, len);//将数据发到socket
             if(strcmp("quit", buf) == 0)
            {
                printf("client quit\n");
                break;
            }
            
            //3.将发出的数据再收回来
            s = recvfrom(sock, buf, sizeof(buf)-1, 0, NULL, NULL);//这里知道是谁发的消息，所以参数设置为NULL即可
            if(s > 0)
            {
                buf[s] = 0;
                printf("server echo# %s\n", buf);
            }

        }
    }

    close(sock);
    return 0;
}

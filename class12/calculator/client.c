//客户端,输入两个用于加法计算的数
#include "comm.h"

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

    //发送数据，并收回结果
    char buf[MAXSIZE];
    while(1)
    {
        request r;
        printf("please enter# ");
        fflush(stdout);
        scanf("%d%d", &r.x, &r.y);

        write(sock, &r, sizeof(r));

        response rp;
        read(sock, &rp, sizeof(rp));

        printf("%d+%d = %d\n", r.x, r.y, rp.res);
        
    }
    close(sock);
    return 0;
}

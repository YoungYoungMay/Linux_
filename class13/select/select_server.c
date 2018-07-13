//select基于TCP编写的网络服务器
//select可以实现同时监视多个文件描述符的状态的变化
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>

#define MAX_FD sizeof(fd_set)*8//可监控的文件描述符个数,取决于fd_set的值
#define INIT -1

static void array_init(int fd_array[], int num)//将array初始化
{
    int i = 0;
    for(; i<num; ++i)
    {
        fd_array[i] = INIT;
    }
}

static int array_add(int fd_array[], int num, int fd)//
{
    int i = 0;
    for(; i<num; ++i)
    {
        if(fd_array[i] == INIT)
        {
            fd_array[i] = fd;
            return 0;
        }
    }
    return -1;
}

static void array_del(int fd_array[], int num, int index)
{
    if(index < num && index > 0)
    {
        fd_array[index] = INIT;
    }
}

int set_rfds(int fd_array[], int num, fd_set* rfds)
{
    int i = 0;
    int max_fd = INIT;
    printf("select:");
    for(; i<num; ++i)
    {
        if(fd_array[i] > INIT)
        {
            printf("%d ", fd_array[i]);
            FD_SET(fd_array[i], rfds);//设置rdfs中的相关fd_array[i]位
            if(max_fd < fd_array[i])
            {
                max_fd = fd_array[i];
            }
        }
    }
    printf("\n");
    return max_fd;
}

int service(int fd_array[], int num, fd_set* rfds)
{
    int i = 0;
    for(; i<num; ++i)
    {
        if(fd_array[i] > INIT && FD_ISSET(fd_array[i], rfds))//
        {
            int fd = fd_array[i];
            if(i == 0)//listen sock ready
            {
                struct sockaddr_in client;
                socklen_t len = sizeof(client);
                int new_sock = accept(fd, (struct sockaddr*)&client, &len);
                if(new_sock < 0)
                {
                    perror("accept");
                    continue;
                }
                printf("get a new client, [%s: %d]\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                if(array_add(fd_array, num, new_sock) < 0)
                {
                    printf("server is busy\n");
                    close(new_sock);
                }
            }
            else
            {
                char buf[1024] = {0};
                ssize_t s = read(fd, buf, sizeof(buf)-1);
                if(s > 0)
                {
                    buf[s] = 0;
                    printf("client: %s\n", buf);
                }
                else if(s == 0)
                {
                    close(fd);
                    printf("client quit!\n");
                    array_del(fd_array, num , i);
                }
                else
                {
                    perror("read");
                    close(fd);
                    array_del(fd_array, num, i);
                }
            }
        }
    }
}

int startup(int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(2);
    }
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sock,(struct sockaddr*)&local, sizeof(local)) < 0)
    {
        perror("bind");
        exit(3);
    }
    
    if(listen(sock, 5) < 0)
    {
        perror("listen");
        exit(4);
    }
    return sock;
}


//./server 8080
int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s [port(>1024)]\n", argv[0]);
        return 1;
    }

    int listen_sock = startup(atoi(argv[1]));
    int fd_array[MAX_FD];//用于保存放到select中的文件描述符
    array_init(fd_array, MAX_FD);
    array_add(fd_array, MAX_FD, listen_sock);
    
    fd_set rfds;//select函数中读、写、异常事件对应参数的类型
    int max_fd = 0;
    for(; ;)
    {
        struct timeval timeout = {5, 0};//第一个参数为秒，第二个参数为微妙
        FD_ZERO(&rfds);//清除rdfs中的所有位，相当于重置
        max_fd = set_rfds(fd_array, MAX_FD, &rfds);
        switch(select(max_fd+1, &rfds, NULL, NULL, NULL))//关注读事件
        {
            case 0://select返回0表示超时，无返回
                printf("select timeout...\n");
                break;
            case -1://返回-1，说明调用出错
                perror("select");
                break;
            default://调用成功返回文件描述词状态已改变的个数
                service(fd_array, MAX_FD, &rfds);//成功就进行服务
                break;
        }
    
    }
    return 0;
}

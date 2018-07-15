//epoll基于TCP编写的网络服务器
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#define MAX 128

static int startup(int port)
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
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(port);

    if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
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

static void serviceIO(int epfd, struct epoll_event* revs, int size, int listen_sock)
{//读/写事件就绪
    int i = 0;
    struct epoll_event ev;
    for(; i<size; ++i)
    {
        int fd = revs[i].data.fd;
        if(revs[i].events & EPOLLIN)//读事件就绪
        {
            if(fd == listen_sock)//accept
            {
                //1.将已就绪的事件accept上来
                //2.将accept上来的事件添加到epoll模型中
                struct sockaddr_in client;
                socklen_t len = sizeof(client);
                int new_fd = accept(fd, (struct sockaddr*)&client, &len);
                if(new_fd < 0)
                {
                    perror("accept");
                    continue;
                }

                printf("get a new client[%s:%d]\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

                ev.events = EPOLLIN;
                ev.data.fd = new_fd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, new_fd, &ev);
            }
            else//普通读就绪事件
            {
                //读数据
                char buf[1024];
                ssize_t s = read(fd, buf, sizeof(buf));
                if(s > 0)//拿到数据
                {
                    buf[s] = 0;
                    printf("client:>%s\n", buf);
                    //使它又关心写
                    ev.events = EPOLLOUT;
                    ev.data.fd = fd;
                    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
                }
                else if(s == 0)//对端关闭
                {
                    printf("client quit\n");
                    close(fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                }
                else
                {
                    perror("read");
                    close(fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                }
            }
        }
        //一个fd可能既读事件就绪也写事件就绪，所以这里不用else if判断
        if(revs[i].events & EPOLLOUT)//写事件就绪
        {
            const char* msg = "HTTP/1.0 200 OK\r\n\r\n<html><h1>hello epoll success:)</h1></html>";
            write(fd, msg, strlen(msg));
            close(fd);
            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
        }
    }

}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s [port]\n", argv[0]);
        return 1;
    }

    int listen_sock = startup(atoi(argv[1]));

    //创建epoll模型
    int epfd = epoll_create(256);
    if(epfd < 0)
    {
        perror("epoll_create");
        return 5;
    }

    //向epoll模型中添加新的fd、关心的事件
    struct epoll_event ev;
    ev.events = EPOLLIN;//关心读事件
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_sock, &ev);

    struct epoll_event revs[MAX];
    int size = 0;
    for(;;)
    {
        switch(size = (epoll_wait(epfd, revs, MAX, -1)))//阻塞式等待
        {
            case -1:
                perror("epoll_wait");
                break;
            case 0:
                printf("timeout...\n");
                break;
            default://一定有就绪事件
                serviceIO(epfd, revs, size, listen_sock);//size表示有多少个就绪事件
                break;
        }
    }

    return 0;
}

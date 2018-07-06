#include "comm.c"

int main()
{
    //printf("## server: ##\n");
    int msgid = CreateMsgQueue();//创建一个消息队列
    char buf[1024]; 
    printf("## server: ##\n");
    while(1)
    {
        //接收消息
        int ret = ReceveMsg(msgid, CLIENT_TYPE, buf);//接收队列第一条type为CLIENT_TYPE的消息
        if(ret == 0)
            printf("client# %s\n",buf);
        else
            printf("receve error\n");
        //发送消息
        printf("Please Enter# ");
        fflush(stdout);
        ssize_t s = read(0, buf, sizeof(buf)-1);
        if(s > 0)
        {
            buf[s] = 0;
            int ret = SendMsg(msgid, SERVER_TYPE, buf);//发送type为SERVER_TYPE的消息
            if(ret == 0)
                printf("send done!\n");
            else
                printf("send error\n");
        }
        else
            perror("read");
    }
    DestroyMsgQueue(msgid);
    return 0;
}

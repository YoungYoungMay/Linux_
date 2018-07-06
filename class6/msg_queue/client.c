#include "comm.c"

int main()
{
    printf("## client: ##\n");
    int msgid = GetMsgQueue();//server已创建一个消息队列，所以这里只需获得已有消息队列即可
    char buf[1024];
    while(1)
    {
       //发送消息
        printf("Please Enter# ");
        fflush(stdout);//printf未遇到回车换行，上句不能自己输出到屏幕
        ssize_t s = read(0, buf, sizeof(buf)-1);
        if(s > 0)
        {
            buf[s] = 0;
            int ret = SendMsg(msgid, CLIENT_TYPE, buf);//发送type为CLIENT_TYPE的消息
            if(ret == 0)
                printf("send done!\n");
            else
                printf("send error\n");
        }
        else
            perror("read");
        //接收消息
        int ret = ReceveMsg(msgid, SERVER_TYPE, buf);//接收队列第一条type为SERVER_TYPE的消息
        if(ret == 0)
            printf("client# %s\n",buf);
        else
            printf("receve error\n"); 
    }
    //DestroyMsgQueue(msgid);
    return 0;
}

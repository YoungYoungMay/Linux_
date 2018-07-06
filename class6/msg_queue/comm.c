//实现消息队列发消息、接消息等相关功能的函数
#include "comm.h"

static int CommMsgQueue(int flags)
{
    key_t key = ftok(PATHNAME,PROJ_ID);//获得一个key值
    if(key == -1)//创建失败
    {   
        perror("ftok");
        return 1;
    }
    int msgid = msgget(key, flags);//创建消息队列，成功返回其标识码
    if(msgid == -1)//创建消息队列失败
    {
        perror("msgget");
        return 2;
    }
    return msgid;//将成功创建的消息队列标识码传回去
}

int CreateMsgQueue()//创建消息队列
{//参数中两个一起使用用于获得一个全新的消息队列
    return CommMsgQueue(IPC_CREAT | IPC_EXCL | 0666);//0666也可以不要，但是进行后面操作时可能会有权限问题
}

int GetMsgQueue()//获得消息队列
{
    return CommMsgQueue(IPC_CREAT);//该参数用来获取已有消息队列
}

int SendMsg(int msgid, int who, char* msg)//发送消息
{
    struct msgbuf buf;//创建一个消息结构
    buf.mtype = who;//传送消息的类型
    strcpy(buf.mtext,msg);//传送的消息
    if(msgsnd(msgid, (void*)&buf, sizeof(buf.mtext), 0) < 0)//将消息发送到消息队列里失败
    {
        perror("msgsnd");
        return 3;
    }
    return 0;
}

int ReceveMsg(int msgid, int rcvType, char out[])//接收消息
{//rcvType实现接收优先级的简单形式
    struct msgbuf buf;
    if(msgrcv(msgid, (void*)&buf, sizeof(buf.mtext),rcvType, 0) < 0)//从消息队列里拿消息失败
    {
        perror("msgrcv");
        return 4;
    }
    strcpy(out, buf.mtext);//将接收到的消息保存起来，输出型参数
    return 0;
}

int DestroyMsgQueue(int msgid)//销毁消息队列
{   
    if(msgctl(msgid, IPC_RMID, NULL) < 0)//清除消息队列失败
    {
        perror("msgctl");
        return 5;
    }
    return 0;
}

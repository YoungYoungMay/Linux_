#include "seqqueue.h"

void SeqQueueInit(SeqQueue* q)//初始化
{
    if(q == NULL)//非法操作
        return;
    q->size = 0;
    q->head = 0;
    q->tail = 0;
    return;
}

void SeqQueueDestroy(SeqQueue* q)//销毁队列
{
    if(q == NULL)
        return;
    q->size = 0;
    q->head = 0;
    q->tail = 0;
    return;
}

void SeqQueuePush(SeqQueue* q,SeqQueueType value)//入队列(尾插)
{
    if(q == NULL)
        return;
    if(q->size >= MAX_SIZE)//队列已满
        return;
    q->data[q->tail++] = value;
    if(q->tail >= MAX_SIZE)//tail走到最后，就再指向头(只要size<MAX_SIZE)
        q->tail = 0;
    q->size++;
    return;
}

void SeqQueuePop(SeqQueue* q)//出列队(头删)
{
    if(q == NULL)//非法操作
        return;
    if(q->size == 0)//空队
        return;
    q->head++;
    if(q->head >= MAX_SIZE)//别忘记！！！
        q->head = 0;
    q->size--;
    return;
}

int SeqQueueGetFront(SeqQueue* q,SeqQueueType* value)//取队首元素
{
    if(q == NULL)
        return 0;
    if(q->size == 0)
        return 0;
    *value = q->data[q->head];
    return 1;
}

void SeqQueuePrint(SeqQueue* q,const char* msg)//打印
{
    printf("[%s]\n",msg);
    if(q == NULL)
        return;
    if(q->size == 0)//空队列
    {
        printf("空队\n");
        return;
    }
    size_t i = q->head;
    if(q->tail <= q->head)//tail在head前
    {
        for(; i<MAX_SIZE; ++i)
            printf("[%c] ",q->data[i]);
        for(i=0; i<q->tail; ++i)//q->tail指向最后一个元素的下一个位置
            printf("[%c] ",q->data[i]);
    }
    else//tail在head后
    {
        for(i=q->head; i<q->tail; ++i)
            printf("[%c] ",q->data[i]);
    }
    printf("\n");
    return;
}

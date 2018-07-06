//顺序队列
#pragma once

#include <stdio.h>
#include <stddef.h>

#define SHOW_NAME printf("\n=================%s==================\n",__FUNCTION__);
#define MAX_SIZE 5

typedef char SeqQueueType;

typedef struct SeqQueue//整个队列的元素是从head到tail，
{           //由于出队是头删，所以前面可能会有空，所以当tail走到最后时，tail再指向0，只要size与MAX_SIZE不相等，就还有空间
    SeqQueueType data[MAX_SIZE];
    size_t head;//队列第一个元素下标
    size_t tail;//队列最后一个元素的下标
    size_t size;//用了多少个元素的空间
}SeqQueue;

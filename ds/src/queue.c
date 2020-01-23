/***************************************
* DATA STRUCTURES: QUEUE               *
* CODE BY:         SASHA LIMAREV       *   
* REVIEWED BY:     YOAV KLEIN          *                          
****************************************/ 
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "../include/sll.h" /* SLL header file */
#include "../include/queue.h" /* Queue header file */

struct queue
{
	sll_t *list;
};

queue_t *QueueCreate()
{
   queue_t *new_queue = (queue_t *)malloc(sizeof(queue_t));
   if(NULL == new_queue)
   {
        return 0;
   } 

   new_queue->list = (sll_t *)SLLCreate();
   if(NULL == new_queue->list)
   {
        free(new_queue);
        return NULL;
   }

    return new_queue;
}

void QueueDestroy(queue_t *queue)
{
    assert(NULL != queue);

    SLLDestroy(queue->list);
    free(queue);    
}

int QueueIsEmpty(const queue_t *queue)
{
    assert(NULL != queue);

    return SLLIsEmpty(queue->list);
}

size_t QueueSize(const queue_t *queue)
{
    assert(NULL != queue);

    return SLLSize(queue->list);
}

int QueueEnqueue(queue_t *queue, const void *data)
{
    it_t push_status = SLLPushBack(queue->list, data);

    assert(NULL != queue);

    return SLLEnd(queue->list) == push_status;
}

void QueueDequeue(queue_t *queue)
{
     assert(NULL != queue);

     SLLPopFront(queue->list);
}

void *QueuePeek(const queue_t *queue)
{
    assert(NULL != queue);

    return SLLGetData(SLLBegin(queue->list));
}

queue_t *QueueAppend(queue_t *dest, queue_t *src)
{
    it_t src_from = SLLBegin(src->list);
    it_t src_end = SLLEnd(src->list);

    assert(NULL != dest && NULL != src);

    for(; src_from != src_end; src_from = SLLNext(src_from))
    {
        QueueEnqueue(dest, SLLGetData(src_from));
    }

    return dest;
}

#ifndef QUEUE_H
#define QUEUE_H


typedef struct queue queue_t;

queue_t *QueueCreate();

void QueueDestroy(queue_t *queue);

int QueueIsEmpty(const queue_t *queue);

int QueueEnqueue(queue_t *queue, const void *data);

void QueueDequeue(queue_t *queue);

void *QueuePeek(const queue_t *queue);

size_t QueueSize(const queue_t *queue);

queue_t *QueueAppend(queue_t *dest, queue_t *src);

#endif

/***************************************
* DATA STRUCTURES: PRIORITY QUEUE      *
* CODE BY:         SASHA LIMAREV       *   
* REVIEWED BY:     ANDREI PLAKHOTA     *                          
****************************************/ 
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "hpq.h" /* priority queue header file */

struct pq
{
    heap_t *heap;
};

int PQErase(pq_t *pq, const void *key, pqcmp_f ShouldErase)
{
    return HeapRemove(pq->heap, ShouldErase, key);
}

void PQClear(pq_t *pq)
{
    assert(NULL != pq);

    while(!PQIsEmpty(pq))
    {
        PQDequeue(pq);
    }
}

void *PQPeek(const pq_t *pq)
{
    assert(NULL != pq);

    return HeapPeek(pq->heap);
}

size_t PQSize(const pq_t *pq)
{
    assert(NULL != pq);

    return HeapSize(pq->heap);
}

int PQDequeue(pq_t *pq)
{
    assert(NULL != pq);

    HeapPop(pq->heap);

    return 0;
}
/* heap must not be null.r eturns 0
   on success, 1 on failure */

int PQEnqueue(pq_t *pq, void *data)
{
    assert(NULL != pq);

    return 0 == HeapPush(pq->heap, data) ? 0 : 1;
}

void PQDestroy(pq_t *pq)
{
    assert(NULL != pq);

    HeapDestroy(pq->heap);
    free(pq);
}

int PQIsEmpty(const pq_t *pq)
{
    assert(NULL != pq);

    return HeapIsEmpty(pq->heap); 
}

pq_t *PQCreate(pqpriority_f PriorityFunction)
{
    pq_t *new_pq = (pq_t *)malloc(sizeof(pq_t));
    if(NULL == new_pq)
    {
        return NULL;
    }

    new_pq->heap = HeapCreate(PriorityFunction);
    if(NULL == new_pq->heap)
    {
        free(new_pq);
    }

    return new_pq;
}

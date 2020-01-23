/***************************************
* DATA STRUCTURES: PRIORITY QUEUE      *
* CODE BY:         SASHA LIMAREV       *   
* REVIEWED BY:     ANDREI PLAKHOTA     *                          
****************************************/ 
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "pq.h" /* priority queue header file */

struct pq
{
	srtl_t *list;
};

int PQErase(pq_t *pq, const void *key, pqcmp_f ShouldErase)
{
    it_t start = NULL;
    it_t end = NULL;
    it_t where = NULL;; 
    
    assert(NULL != pq && NULL != ShouldErase);

    start = SrtLBegin(pq->list);
    end = SrtLEnd(pq->list);
    where = SrtLFind(start, end, ShouldErase, key, NULL); 

    if(where == end)
    {
        return 1;
    }
    else
    {
        SrtLErase(where);
        return 0;
    }
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

    return SrtLGetData(SrtLBegin(pq->list));
}

size_t PQSize(const pq_t *pq)
{
    assert(NULL != pq);

    return SrtLSize(pq->list);
}

int PQDequeue(pq_t *pq)
{
    assert(NULL != pq);

    SrtLPopFront(pq->list); 

    return 0;
}

int PQEnqueue(pq_t *pq, void *data)
{
    assert(NULL != pq);

    return NULL != SrtLInsert(pq->list, data) ? 0 : 1;
}

void PQDestroy(pq_t *pq)
{
    assert(NULL != pq);

    SrtLDestroy(pq->list);
    free(pq);
}

int PQIsEmpty(const pq_t *pq)
{
    assert(NULL != pq);

    return SrtLIsEmpty(pq->list); 
}

pq_t *PQCreate(pqpriority_f PriorityFunction)
{
    pq_t *new_pq = (pq_t *)malloc(sizeof(pq_t));
    if(NULL == new_pq)
    {
        return NULL;
    }

    new_pq->list = SrtLCreate(PriorityFunction, NULL);
    if(NULL == new_pq->list)
    {
        free(new_pq);
    }

    return new_pq;
}

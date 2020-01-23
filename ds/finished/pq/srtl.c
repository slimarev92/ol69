/***************************************
* DATA STRUCTURES: SORTED LIST         *
* CODE BY:         SASHA LIMAREV       *   
* REVIEWED BY:     ELDAD CARIN         *                          
****************************************/ 
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "srtl.h" /* Sorted list header file */

struct srtl
{
	dll_t *list;
	isbefore_f is_before;
	const void *params;
};

struct srtl_node
{
    it_t node;
};

void SrtLMerge(srtl_t *dest, srtl_t *src)
{
    it_t left = NULL;
    it_t right = NULL;
    it_t curr = NULL;
    it_t dst_end = NULL;
    it_t src_end = NULL;
    scmp_f is_before = NULL;    
    const void *params = NULL;

    assert(NULL != dest && NULL != src);

    left = SrtLBegin(src);
    right = left;
    curr = SrtLBegin(dest);
    dst_end = SrtLEnd(dest);
    src_end = SrtLEnd(src);

    is_before = src->is_before;
    params = src->params;

    while(!SrtLIsEmpty(src) && curr != dst_end)
    {      
        if(curr != dst_end && !(is_before(SrtLGetData(curr), 
                                    SrtLGetData(left), params)))
        {
            curr = SrtLNext(curr);
        }
        else if(right != src_end && (is_before(SrtLGetData(curr), 
                                    SrtLGetData(right), params)))
        {
            right = SrtLNext(right);
        }
        else
        {
            DLLSpliceBefore(curr, left, right);
            curr = SrtLNext(curr);
            left = right;
        }
    }

     right = src_end;
     DLLSpliceBefore(curr, left, right);
}

int SrtLForEach(sit_t from, sit_t to, sact_f action,const void *params)
{
    assert(NULL != from && NULL != to);

    return DLLForEach(from, to, action, params);
}

it_t SrtLFind(sit_t from, sit_t to, cmp_f compare,
              const void *key, const void *params)
{
    assert(NULL != from && NULL != to);

    for(; from != to; from = DLLNext(from))
    {
        if(1 == compare(DLLGetData(from), key, params))
        {
            return from;       
        }
    }

    return to;
}

int SrtLIsSame(const it_t iter1, const it_t iter2)
{    
    return DLLIsSameIter(iter1, iter2);
}

it_t SrtLErase(it_t where)
{
    assert(NULL != where);

    return DLLErase(where);
}

void SrtLPopFront(srtl_t *srtl)
{
    assert(NULL != srtl);

    DLLPopFront(srtl->list);
}

void SrtLPopBack(srtl_t *srtl)
{
     assert(NULL != srtl);

    DLLPopBack(srtl->list);
}

int SrtLIsEmpty(const srtl_t *srtl)
{
     assert(NULL != srtl);

    return DLLIsEmpty(srtl->list);
}

size_t SrtLSize(const srtl_t *srtl)
{
     assert(NULL != srtl);

    return DLLSize(srtl->list);
}

it_t SrtLBegin(const srtl_t *srtl)
{
     assert(NULL != srtl);

    return DLLBegin(srtl->list);
}

it_t SrtLEnd(const srtl_t *srtl)
{
     assert(NULL != srtl);

    return DLLEnd(srtl->list);
}

it_t SrtLNext(const it_t iter)
{
     assert(NULL != iter);

    return DLLNext(iter);
}

it_t SrtLPrev(const it_t iter)
{
     assert(NULL != iter);

    return DLLPrev(iter);
}

void *SrtLGetData(const it_t iter)
{
     assert(NULL != iter);

    return DLLGetData(iter);
}

sit_t SrtLInsert(srtl_t *srtl, void *data)
{
    it_t curr = NULL;
    it_t next = NULL;
    it_t end = NULL;

    assert(NULL != srtl);

    curr = SrtLPrev(SrtLBegin(srtl));
    next = SrtLNext(curr);
    end = SrtLEnd(srtl);
    
    while(next != end && (0 == srtl->is_before(SrtLGetData(next), data,  srtl->params)))
    {
        curr = next;
        next = SrtLNext(curr);
    }

    return DLLInsert(srtl->list, curr, data); 
}

srtl_t *SrtLCreate(isbefore_f user_is_before, const void *user_params)
{
    srtl_t *new_srtl = NULL;
    dll_t *new_list = NULL;

    assert(NULL != user_is_before);

    new_srtl = (srtl_t *)malloc(sizeof(srtl_t));
    if(NULL == new_srtl)
    {
        return NULL;
    }

    new_list = DLLCreate();
    if(NULL == new_list)
    {
        free(new_srtl);
        return NULL;
    }

    new_srtl->list = new_list;
    new_srtl->is_before = user_is_before;
    new_srtl->params = (void *)user_params;
    
    return new_srtl;
}

void SrtLDestroy(srtl_t *srtl)
{
     assert(NULL != srtl);

    DLLDestroy(srtl->list);
    free(srtl);
}

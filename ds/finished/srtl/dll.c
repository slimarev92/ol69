/***************************************
* DATA STRUCTURES: DOUBLY LINKED LIST  *
* CODE BY:         SASHA LIMAREV       *   
* REVIEWED BY:     ROI KRAMER          *                          
****************************************/ 

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "dll.h" /* DLL header file */

struct dll_node               
{
	void *data;
	struct dll_node *next;
	struct dll_node *prev;
};

struct dll
{
	struct dll_node *head;
	struct dll_node *tail;
};

typedef struct dll_node dll_node_t;

dll_t *DLLCreate()
{
    dll_node_t *head_node = NULL;
    dll_node_t *tail_node = NULL;

    dll_t *new_dll = (dll_t *)malloc(sizeof(dll_t));
    if(NULL == new_dll)
    {
        return NULL;
    }

    head_node = (dll_node_t *)malloc(sizeof(dll_node_t));
    if(NULL == head_node)
    {
        free(new_dll);
        return NULL;
    }

    tail_node = (dll_node_t *)malloc(sizeof(dll_node_t));
    if(NULL == tail_node)
    {
        free(new_dll);
        free(head_node);
        return NULL;
    }

    new_dll->head = head_node;
    new_dll->tail = tail_node;

    head_node->data = (void *)0xDEADBEEF;
    tail_node->data = (void *)0xDEADBEEF;

    head_node->next = tail_node;
    head_node->prev = NULL;
    tail_node->prev = head_node;
    tail_node->next = NULL;
    
    return new_dll;
}

void DLLDestroy(dll_t *dll)
{
    it_t end = NULL;

    assert(NULL != dll);

    end = DLLEnd(dll);

    while(DLLBegin(dll) != end)
    {   
        DLLPopBack(dll);
    }

    free(dll->head);
    free(dll->tail);
    free(dll);
}

it_t DLLBegin(const dll_t *dll)
{
    assert(NULL != dll);    

    return dll->head->next;
}

it_t DLLEnd(const dll_t *dll)
{
    assert(NULL != dll);    

    return dll->tail;
}

it_t DLLNext(const it_t iter)
{
    assert(NULL != iter);    

    return iter->next;
}

it_t DLLPrev(const it_t iter)
{
    assert(NULL != iter);    

    return iter->prev;
}

it_t DLLFind(it_t from, it_t to, cmp_f compare, const void *key, const void *params)
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

void DLLPopBack(dll_t *dll)
{
    DLLErase(DLLPrev(DLLEnd(dll)));
}

void DLLPopFront(dll_t *dll)
{
    DLLErase(DLLBegin(dll));
}

it_t DLLErase(it_t where)
{
    dll_node_t *next = NULL;

    assert(NULL != where);

    next = where->next; 

    where->prev->next = DLLNext(where);
    where->next->prev = DLLPrev(where);

    free(where);
    return next;
}

it_t DLLInsert(dll_t *dll, it_t iter, void *data)
{
    dll_node_t *new_node = NULL;

    assert(NULL != dll && NULL != iter);

    new_node = (dll_node_t *)malloc(sizeof(dll_node_t));
    if(NULL != new_node)
    {
        new_node->data = data;

        new_node->prev = iter;  
        new_node->next = DLLNext(iter);
        new_node->next->prev = new_node;

        iter->next = new_node;      
    }

    return new_node;
}

it_t DLLPushFront(dll_t *dll, void *data)
{
    return DLLInsert(dll, dll->head, data);
}

int DLLIsEmpty(const dll_t *dll)
{
    assert(NULL != dll);

    return DLLBegin(dll) == DLLEnd(dll);
}

size_t DLLSize(const dll_t *dll)
{
    size_t counter = 0;
    it_t curr = NULL;
    it_t end = NULL;

    assert(NULL != dll);

    end = DLLEnd(dll);

    for(curr = DLLBegin(dll); curr != end; curr = DLLNext(curr))
    {
        ++counter;
    }

    return counter;
}

it_t DLLPushBack(dll_t *dll, void *data)
{   
    return DLLInsert(dll, DLLPrev(DLLEnd(dll)), data);
}

int DLLIsSameIter(const it_t iter1,const it_t iter2)
{
    assert(NULL != iter1 && NULL != iter2);

    return iter1 == iter2;
}

void *DLLGetData(const it_t iter)
{
    assert(NULL != iter);

    return iter->data;
}


void DLLSpliceBefore(it_t where, it_t from, it_t to)
{
    dll_node_t *prec_where = NULL;
    dll_node_t *prec_from = NULL;
    dll_node_t *prec_to = NULL;

    assert(NULL != where && NULL != from && NULL != to);

    prec_where = DLLPrev(where);
    prec_from = DLLPrev(from);
    prec_to = DLLPrev(to);

    prec_where->next = from;
    from->prev = prec_where;

    prec_to->next = where;
    where->prev = prec_to;

    prec_from->next = to;
    to->prev = prec_from;
}

int DLLForEach(it_t from, it_t to, act_f action, const void* params)
{
    int curr_status = 0;

    assert(NULL != from && NULL != to);

    for(; from != to; from = DLLNext(from))
    {
        curr_status = action(DLLGetData(from), params);  

        if(0 != curr_status)
        {
            return curr_status;
        }
    }
    
    return 0;
}


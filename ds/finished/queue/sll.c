/***************************************
* DATA STRUCTURES: SINGLY LINKED LIST  *
* CODE BY:         SASHA LIMAREV       *   
* REVIEWED BY:     ALEX ZILBERMAN      *                          
****************************************/ 

#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf */

#include "sll.h" /* SLL header file */

struct node
{
    void *data;
    struct node *next;
};

struct sll
{
    struct node *head;
    struct node *tail;
};

typedef struct node node_t;

static node_t *SLLGetPreceedingNode(const sll_t *sll, node_t *iter);

sll_t *SLLFLip(sll_t *sll)
{
    node_t *curr_node = sll->head->next;
    node_t *next_node = curr_node->next;
    node_t *next_node_next = NULL;
    node_t *end = sll->tail;

    while(next_node->next != end)
    {
        next_node_next = next_node->next; 

        next_node->next = curr_node;
        curr_node = next_node;
        next_node = next_node_next;
    }
    
    next_node->next = curr_node;
    curr_node = sll->head->next;

    sll->head->next = next_node;
    curr_node->next  = end;

    return sll;
}

int SLLForEach(it_t from, it_t to, const void *param, act_f action)
{
    int curr_status = 0;
    
    for(; from != to; from = SLLNext(from))
    {
        curr_status = (*action)(from->data, param);  

        if(0 != curr_status)
        {
            return curr_status;
        }
    }
    
    return 0;  
}

node_t *SLLInsertAfter(sll_t *sll, node_t *node, const void *data)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if(NULL == new_node)
    {
        return sll->tail;
    }

    new_node->data = (void *)data;
    new_node->next = node->next;
    node->next = new_node;

    return new_node;
}

void SLLPopBack(sll_t *sll)
{
    node_t *first = sll->head;
    node_t *second = sll->head->next;
    node_t *end = sll->tail;
    
    while(second->next != end)
    {
        first = second;
        second = first->next;
    }

    first->next = second->next;
    free(second);    
}

void SLLDestroy(sll_t *sll)
{
    node_t *curr = sll->head;
    node_t *end = sll->tail;

    while(curr->next != end)
    {
        SLLPopFront(sll);
    }

    free(sll->head);
    free(sll->tail);
    free(sll);
}

void SLLErase(sll_t *sll, node_t *where)
{
    node_t *preceeding_node = SLLGetPreceedingNode(sll, where);

    preceeding_node->next = where->next;
    free(where);
}

void SLLPopFront(sll_t *sll)
{
    node_t *to_erase = sll->head->next;

    sll->head->next = to_erase->next;
    free(to_erase);
}

it_t SLLFind(it_t from, it_t to, const void *key, cmp_f compare)
{
    for(; from != to; from = SLLNext(from))
    {
        if(0 !=  (*compare)(from->data, key))
        {
            return from;
        }      
    }

    return from;
}

sll_t *SLLCreate()
{
    node_t *new_head = NULL;
    node_t *new_tail = NULL;
    sll_t *new_sll = (sll_t *)malloc(sizeof(sll_t));
    if(NULL == new_sll)
    {
        return NULL;
    }
    
    new_head = (node_t *)malloc(sizeof(node_t));
    if(NULL == new_head)
    {
        free(new_sll);
        return NULL;
    }

    new_head->data = (void *)0xDEADBEEF;

    new_tail = (node_t *)malloc(sizeof(node_t));
    if(NULL == new_tail)
    {
        free(new_sll);
        free(new_head);
        return NULL;
    }
    
    new_tail->data = (void *)0xDEADBEEF;
    new_head->next = new_tail;
    new_tail->next = NULL;
    
    new_sll->head = new_head;
    new_sll->tail = new_tail;

    return new_sll;
}

node_t *SLLPushFront(sll_t *sll, const void *data)
{
    return SLLInsertAfter(sll, sll->head, data);
}

static node_t *SLLGetPreceedingNode(const sll_t *sll, node_t *node)
{
    node_t *curr = sll->head;

    while(curr->next != node)
    {
        curr = curr->next;
    }

    return curr;
}

node_t *SLLPushBack(sll_t *sll, const void *data)
{
    node_t *one_before_last = NULL;

    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if(NULL == new_node)
    {
        return sll->tail;
    }

    one_before_last = SLLGetPreceedingNode(sll, SLLEnd(sll));

    new_node->data = (void *)data;
    new_node->next = sll->tail;
    one_before_last->next = new_node;
   
    return new_node;
}

size_t SLLSize(const sll_t *sll)
{
    size_t size = 0;
    it_t iter = NULL;
    it_t end = SLLEnd(sll);

    for(iter = SLLBegin(sll); iter != end; iter = SLLNext(iter))
    {
        ++size;
    }
    
    return size;
}

int SLLIsEmpty(const sll_t *sll)
{
    return sll->head->next == sll->tail;
}

void *SLLGetData(it_t iter)
{
    return iter->data;
}

it_t SLLBegin(const sll_t *sll)
{
    return sll->head->next;
}

it_t SLLEnd(const sll_t *sll)
{
    return sll->tail;
}

it_t SLLNext(it_t iter)
{
    return iter->next;
}

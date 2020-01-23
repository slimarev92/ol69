#ifndef SLL_H
#define SLL_H

typedef struct node *it_t; 
typedef struct sll sll_t;
typedef int (*cmp_f)(const void *data, const void *key);
typedef int (*act_f)(void *data, const void *arg);  

sll_t *SLLCreate();

void SLLDestroy(sll_t *sll);

it_t SLLInsertAfter(sll_t *sll, it_t iter, const void *data); 

void SLLPopBack(sll_t *sll);

void SLLPopFront(sll_t *sll);

it_t SLLPushBack(sll_t *sll, const void *data);

it_t SLLPushFront(sll_t *sll, const void *data);

void *SLLGetData(it_t iter);

it_t SLLBegin(const sll_t *sll);

it_t SLLEnd(const sll_t *sll);

it_t SLLNext(it_t iter);

int SLLForEach(it_t from, it_t to, const void *param, act_f action);

it_t SLLFind(it_t from, it_t to, const void *key, cmp_f compare);

int SLLIsEmpty(const sll_t *sll);

size_t SLLSize(const sll_t *sll);

sll_t *SLLFLip(sll_t *sll);

void SLLErase(sll_t *sll, it_t where);

int SLLHasLoop(const sll_t *sll);

it_t SLLFindIntersection(const sll_t *sll1, const sll_t *sll2);

#endif

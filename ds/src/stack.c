/*****************************
* DS-STACK                   *
* CODE BY:     SASHA LIMAREV *   
* REVIEWED BY: YOAV KLEIN    *                          
******************************/ 
#include <stdlib.h> /* free */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "stack.h"

struct stack
{
    void *current;
    void *end;
    size_t element_size;
    char head[1];
};

void STACKPop(stack_t *stack)
{
    assert(stack != NULL);

    stack->current = (void *)((size_t) stack->current - stack->element_size);
}

void *STACKPeek(const stack_t *stack)
{
    assert(stack != NULL);

    return (void *)((size_t)stack->current - stack->element_size);
}

size_t STACKSize(const stack_t *stack)
{   
    size_t ditance_in_bytes = (size_t)stack->current - (size_t)stack->head;

    assert(stack != NULL);

    return ditance_in_bytes / stack->element_size;
}

stack_t *STACKCreate(size_t n_elements, size_t size_of_element)
{
    size_t n_bytes = (n_elements * size_of_element) + sizeof(stack_t) - 7;
    stack_t *new_stack = NULL;

    assert(0 != n_elements && 0 != size_of_element);

    new_stack = (stack_t *)malloc(n_bytes);
    if(NULL == new_stack)
    {
        return NULL;
    }
    
    new_stack->current = (void *)new_stack->head;
    new_stack->end = (void *)(new_stack->head + n_elements * size_of_element);
    new_stack->element_size = size_of_element;

    return new_stack;
}

int STACKPush(stack_t *stack, const void *element)
{ 
    assert(NULL != stack && NULL != element);

    if((char *)stack->current >= (char *)stack->end)
    {
        return 1;
    }
    
    memcpy(stack->current, element, stack->element_size);

    stack->current = (void *)((char *)stack->current + stack->element_size);

    return 0;
}

void STACKDestroy(stack_t *stack)
{
    free(stack);
}

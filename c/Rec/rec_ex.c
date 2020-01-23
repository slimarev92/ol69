/***************************************
* C:               RECURSION           *
* CODE BY:         SASHA LIMAREV       *   
* REVIEWED BY:     INBAR NAMDAR        *                          
****************************************/ 
#include "recursion.h" /* rec header file */

long IterFibonacci(long n)
{
    long first = 1;
    long second = 1;

    for(; 2 < n; --n)
    {
        second = first + second;
        first = second - first;
    }

    return second;
}

static unsigned int FibRec(unsigned int frst, unsigned int scnd, unsigned int n)
{
    return (n <= 2) ? scnd : FibRec(scnd, frst + scnd, n - 1);
}

long RecurFibonacci(long n)
{
    return FibRec(1, 1, n);
}

size_t RecurStrlen(const char *str)
{
    return ('\0' == *str) ? 0 : 1 + RecurStrlen(str + 1);
}

int RecurStrcmp(const char *str1, const char *str2)
{
    if(*str1 == *str2)
    {
        return ('\0' == *str1) ? 0 : RecurStrcmp(str1 + 1, str2 + 1);
    }
    else
    {
        return (int)*str1 - (int)*str2;
    }
}

char *RecurStrcpy(char *dest, const char *src)
{
    *dest = *src;

    if('\0' == *dest)
    {
        return NULL;
    }
    else
    {
        RecurStrcpy(dest + 1, src + 1);
        return dest;
    }
}

node_t *RecurFlip(node_t *node)
{
    node_t *result = NULL;

    if(NULL == node->next)
    {
        return node;
    }
    else
    {
        result = RecurFlip(node->next);
        node->next->next = node;
        node->next = NULL;
        return result;
    }
}


static int SortTopTwo(stack_t *stack)
{
    int shallow = 0;
    int deep = 0;
    int ret_val = 0;

    shallow = *(int *)STACKPeek(stack);
    STACKPop(stack);
    deep = *(int *)STACKPeek(stack);
    STACKPop(stack);

    if(shallow > deep)
    {
        STACKPush(stack, (void *)&shallow);
        STACKPush(stack, (void *)&deep);
        ret_val = 1;
    }
    else
    {
        STACKPush(stack, (void *)&deep);
        STACKPush(stack, (void *)&shallow);
    }

    return ret_val;
}

void RecurStackSort(stack_t *stack)
{
    int temp = 0;
    static int switched = 0;

    if(1 >= STACKSize(stack))
    {
        switched = 0;
        return;
    }

    switched = SortTopTwo(stack);

    if(switched)
    {
        RecurStackSort(stack);
        switched = SortTopTwo(stack);
    }
    else
    {
        temp = *(int *)STACKPeek(stack);
        STACKPop(stack);
        RecurStackSort(stack);
        STACKPush(stack, (void *)&temp);
    }

    if(0 != switched)
    {
        RecurStackSort(stack);
    }
}


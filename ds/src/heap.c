/***************************************
* DATA STRUCTURES: HEAP                *
* CODE BY:         SASHA LIMAREV       *   
* REVIEWED BY:     ROY KRAMER          *                          
****************************************/ 
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "heap.h" /* heap header file */        
#include "dvec.h" /* dvec header file */

#define START (1)
#define INIT_SIZE (16)
#define L_CHILD(index) (index * 2)
#define R_CHILD(index) (index * 2 + 1)
#define PARENT(index) (index / 2)

struct heap
{
    dvec_t *vector;
    int (*Compare)(const void *data1, const void *data2, const void *params);
};

static void SwapIndices(dvec_t *dvec, size_t i1, size_t i2);
static int CompareWithParent(heap_t *heap, size_t curr_i, size_t parent_i);
static void HeapifyDown(heap_t *heap, size_t index);
static void *GetIndexData(heap_t *heap, size_t index);
static size_t GetLargerIndex(heap_t *heap, size_t i_left, size_t i_right);
static void HeapifyUp(heap_t *heap, size_t curr_i);

static const size_t g_dummy_value = 0xDEADC0DE;

heap_t *HeapCreate(int (*Cmp)(const void *d1, const void *d2, const void *p))
{
    heap_t *new_heap = NULL;
    
    assert(Cmp);

    new_heap = (heap_t *)malloc(sizeof(heap_t));
    if(NULL == new_heap)
    {
        return NULL;
    }

    new_heap->vector = DVECCreate(sizeof(void *), INIT_SIZE);
    if(NULL == new_heap->vector)
    {
        free(new_heap);
        return NULL;
    }

    new_heap->Compare = Cmp;

    /* push dummy value to index 0 to make calculations simpler */
    if(0 != DVECPushBack(new_heap->vector, (void *)&g_dummy_value))
    {
        free(new_heap->vector);
        free(new_heap);
    }

    return new_heap;
}

void HeapDestroy(heap_t *heap)
{
    assert(heap);

    DVECDestroy(heap->vector);
    heap->vector = NULL;

    free(heap);
    heap = NULL;
}

int HeapPush(heap_t *heap, void *data)
{
    assert(heap);

    if(0 != DVECPushBack(heap->vector, &data))
    {
        return 1;
    }
    else
    {
        HeapifyUp(heap, HeapSize(heap));
        return 0;
    }
}

void *HeapPeek(const heap_t *heap)
{
    assert(heap);

    return GetIndexData((heap_t *)heap, START);
}

void HeapPop(heap_t *heap)
{
    size_t last_index =  0;
    
    assert(heap);

    last_index = HeapSize(heap);
    SwapIndices(heap->vector, last_index, START);
    DVECPopBack(heap->vector);

    HeapifyDown(heap, START);
}

int HeapRemove(heap_t *heap, 
               int (*ShouldRemove)(const void *data,
                                   const void *key,
                                   const void *params),
               const void *key)
{
    size_t last_index =  0;
    size_t i = START;
    
    assert(heap);
    assert(ShouldRemove);

    last_index = HeapSize(heap);
    
    for(; i <= last_index; ++i)  
    {
        if(ShouldRemove(GetIndexData(heap, i), key, NULL))
        {
            if(i == last_index)
            {
                DVECPopBack(heap->vector);
                return 0;
            }

            SwapIndices(heap->vector, last_index, i);
            DVECPopBack(heap->vector);

            HeapifyUp(heap, i);
            HeapifyDown(heap, i);
 
            return 0;
        }
    }

    return 1;
}

size_t HeapSize(const heap_t *heap)
{
    assert(heap);

    return DVECSize(heap->vector) - START;
}

int HeapIsEmpty(const heap_t *heap)
{
    assert(heap);

    return 0 == HeapSize(heap);
}

static void HeapifyDown(heap_t *heap, size_t index)
{
    size_t max_index = HeapSize(heap);
    size_t swap_index = index;

    while(index <= PARENT(max_index))
    {
        swap_index = GetLargerIndex(heap, L_CHILD(index), R_CHILD(index));
        swap_index = GetLargerIndex(heap, index, swap_index);

        if(swap_index != index)
        {
            SwapIndices(heap->vector, index, swap_index);
            index = swap_index;
        }
        else
        {
            break;
        }
    }
}

static void HeapifyUp(heap_t *heap, size_t curr_i)
{
    size_t parent_i = PARENT(curr_i);

    while(START != curr_i && 1 == CompareWithParent(heap, parent_i, curr_i))
    {
        SwapIndices(heap->vector, curr_i, parent_i);
        curr_i = parent_i;
        parent_i = PARENT(parent_i);
    }
}

static size_t GetLargerIndex(heap_t *heap, size_t i_left, size_t i_right)
{
    int compare_res = 0;
    size_t heap_size = HeapSize(heap);

    /*  only left child exists */
    if(i_left <= heap_size && i_right > heap_size)
    {
        return i_left;
    }
    /* both children exist */
    else
    {
        compare_res = heap->Compare(GetIndexData(heap, i_left),
                                    GetIndexData(heap, i_right), NULL);

        return (1 == compare_res) ? i_right : i_left;
    }  
}

static void *GetIndexData(heap_t *heap, size_t index)
{
    return *(void **)DVECGetItemAddress(heap->vector, index);
}

static int CompareWithParent(heap_t *heap, size_t curr_i, size_t parent_i)
{
    return heap->Compare(GetIndexData(heap, curr_i),
                         GetIndexData(heap, parent_i), NULL);
}

static void SwapIndices(dvec_t *dvec, size_t i1, size_t i2)
{
    void **data1 = (void **)DVECGetItemAddress(dvec, i1);
    void **data2 = (void **)DVECGetItemAddress(dvec, i2);
    void **temp = *data1;

    *data1 = *data2;
    *data2 = temp;
}
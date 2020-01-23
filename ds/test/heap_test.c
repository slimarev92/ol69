#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "heap.h" 

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[93m"
#define RESET_COLOR "\x1b[0m"

#define HASH_RANGE (729)
#define WORD_HASH_RANGE (72)
#define NEW_BALANCE (23.23)
#define DICT_SIZE (99171)

#define RUN_TEST(test_name) \
    printf("\n%s[Calling: " #test_name "]\n%s", YELLOW, RESET_COLOR); \
    ++g_num_of_tests; \
    \
    if(0 == test_name()) \
    { \
        ++g_successful_tests; \
        printf("%s[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \
    } \
    printf("%s\n", RESET_COLOR);

static int g_num_of_tests = 0;
static int g_successful_tests = 0;

typedef struct holder
{
    int num;
} holder_t;

/* IsData1BeforeData2 function returns 1
   if data2 should be placed higher on the
   heap, otherwise, returns 0. */
static int CompareHolders(const void *d1, const void *d2, const void *params)
{
    holder_t *h1 = (holder_t *)d1;
    holder_t *h2 = (holder_t *)d2;

    (void)params;
  
    return (h2->num) - (h1->num) > 0 ? 1 : 0;
}

static int ShouldRemove(const void *data, const void *key, const void *params)
{
    (void)params;

    return (((holder_t *)data)->num == *(int *)key);
}

static int TestRemove()
{
    static holder_t nums[] = {{100}, {50}, {40}, {30}, {20}, {10}, {0}, {200}};
    heap_t *test_heap = HeapCreate(CompareHolders);
    size_t arr_size = 8;
    size_t i = 0;
    size_t expected_size = 7;
    size_t actual_size = 0;
    int expected_peek = 200;
    int actual_peek = 0;
    int to_remove = 40;
    
    /* push nums to heap */
    for(; arr_size > i; ++i)
    {
        HeapPush(test_heap, (void *)&nums[i]);
    }
    
    /* remove 40 */
    HeapRemove(test_heap, ShouldRemove, (void *)&to_remove);

    /* top should be 200 */
    actual_peek = ((holder_t *)HeapPeek(test_heap))->num; 

    /* check if top is indeed 200 */
    if(actual_peek != expected_peek)
    {
        HeapDestroy(test_heap);
        printf("\t%sLine %d - Expected max %d, actual max %d\n",
                                 RED, __LINE__ - 4, expected_peek, actual_peek);
        return 1;
    }

    /* get heap size after pushes (should be 6) */
    actual_size = HeapSize(test_heap);

    /* check if size is indeed 6 */
    if(actual_size != expected_size)
    {
        HeapDestroy(test_heap);
        printf("\t%sLine %d - Expected size %ld, actual size %ld\n",
                                 RED, __LINE__ - 4, expected_size, actual_size);
        return 1;
    }
   
    HeapDestroy(test_heap);
    return 0;
}

static int TestRemoveLeaf()
{
    static holder_t nums[] = {{100}, {20}, {40}, {10}, {15}, {29}, {30}};
    heap_t *test_heap = HeapCreate(CompareHolders);
    size_t arr_size = 7;
    size_t i = 0;
    size_t expected_size = 4;
    size_t actual_size = 0;
    int expected_peek = 30;
    int actual_peek = 0;
    int to_remove = 10;
    
    /* push nums to heap */
    for(; arr_size > i; ++i)
    {
        HeapPush(test_heap, (void *)&nums[i]);
    }
    
    /* remove 40, pop twice */
    HeapRemove(test_heap, ShouldRemove, (void *)&to_remove);
    HeapPop(test_heap);
    HeapPop(test_heap);


    /* top should be 30 */
    actual_peek = ((holder_t *)HeapPeek(test_heap))->num; 

    /* check if top is indeed 200 */
    if(actual_peek != expected_peek)
    {
        HeapDestroy(test_heap);
        printf("\t%sLine %d - Expected max %d, actual max %d\n",
                                 RED, __LINE__ - 4, expected_peek, actual_peek);
        return 1;
    }

    /* get heap size after pushes (should be 6) */
    actual_size = HeapSize(test_heap);

    /* check if size is indeed 6 */
    if(actual_size != expected_size)
    {
        HeapDestroy(test_heap);
        printf("\t%sLine %d - Expected size %ld, actual size %ld\n",
                                 RED, __LINE__ - 4, expected_size, actual_size);
        return 1;
    }
   
    HeapDestroy(test_heap);
    return 0;
}


static int TestPop()
{
    static holder_t nums[] = {{100}, {50}, {40}, {30}, {20}, {10}, {0}, {200}};
    heap_t *test_heap = HeapCreate(CompareHolders);
    size_t arr_size = 8;
    size_t i = 0;
    size_t expected_size = 6;
    size_t actual_size = 0;
    int expected_peek = 50;
    int actual_peek = 0;
    
    /* push nums to heap */
    for(; arr_size > i; ++i)
    {
        HeapPush(test_heap, (void *)&nums[i]);
    }
    
    /* pop top (200) */
    HeapPop(test_heap);

    /* pop top (100) */
    HeapPop(test_heap);

    /* top should be 50 */
    actual_peek = ((holder_t *)HeapPeek(test_heap))->num; 

    /* check if top is indeed 200 */
    if(actual_peek != expected_peek)
    {
        HeapDestroy(test_heap);
        printf("\t%sLine %d - Expected max %d, actual max %d\n",
                                 RED, __LINE__ - 4, expected_peek, actual_peek);
        return 1;
    }

    /* get heap size after pushes (should be 6) */
    actual_size = HeapSize(test_heap);

    /* check if size is indeed 6 */
    if(actual_size != expected_size)
    {
        HeapDestroy(test_heap);
        printf("\t%sLine %d - Expected size %ld, actual size %ld\n",
                                 RED, __LINE__ - 4, expected_size, actual_size);
        return 1;
    }
   
    HeapDestroy(test_heap);
    return 0;
}

static int TestInsertComplex()
{
    static holder_t nums[] = {{100}, {50}, {40}, {30}, {20}, {10}, {0}, {200}};
    heap_t *test_heap = HeapCreate(CompareHolders);
    size_t arr_size = 8;
    size_t i = 0;
    int expected_peek = 200;
    int actual_peek = 0;
    
    /* push nums to heap */
    for(; arr_size > i; ++i)
    {
        HeapPush(test_heap, (void *)&nums[i]);
    }
    
    /* top should be 200 */
    actual_peek = ((holder_t *)HeapPeek(test_heap))->num; 

    /* check if top is indeed 200 */
    if(actual_peek != expected_peek)
    {
        HeapDestroy(test_heap);
        printf("\t%sLine %d - Expected max %d, actual max %d\n",
                                 RED, __LINE__ - 4, expected_peek, actual_peek);
        return 1;
    }
   
    HeapDestroy(test_heap);
    return 0;
}

static int TestSort()
{
    static holder_t nums[] = {
                                {2},   {76},  {12},  {71},
                                {89},  {35},  {21},  {6},
                                {30},  {97},  {9},   {77},
                                {58},  {6},   {83},  {96},
                                {20},  {79},  {71},  {79}
                             };
    heap_t *test_heap = HeapCreate(CompareHolders);
    size_t i = 0;
    size_t arr_size = 20;
    static int sorted_arr[20];

    /* push nums to heap */
    for(; arr_size > i; ++i)
    {
        HeapPush(test_heap, (void *)&nums[i]);
    }

    /* peek to arr, then pop (hopefully in sorted oreder) */
    for(i = 0; i < arr_size; ++i)
    {
        sorted_arr[i] = ((holder_t *)HeapPeek(test_heap))->num; 
        HeapPop(test_heap);
    }

    /* check if array is sorted */
    for(i = 0; i < arr_size - 1; ++i)   
    {
        if(sorted_arr[i] < sorted_arr[i + 1])
        {
            HeapDestroy(test_heap);
            printf("\t%sLine %d - arr[%ld] - %d < arr[%ld] - %d\n",
                 RED, __LINE__ - 4, i, sorted_arr[i], i + 1, sorted_arr[i + 1]);
        return 1;
        } 
    }
    
    HeapDestroy(test_heap);
    return 0;
}

static int TestInsertSizeIsEmptySimple()
{
    static holder_t nums[] = {{100}, {50}, {40}, {30}, {20}, {10}, {0}};
    heap_t *test_heap = HeapCreate(CompareHolders);
    size_t i = 0;
    size_t arr_size = 7;
    size_t expected_size = arr_size;
    size_t actual_size = 0;

    /* heap should be empty */
    if(1 != HeapIsEmpty(test_heap))
    {
        HeapDestroy(test_heap);
        printf("\t%sLine %d - Expected empty heap\n", RED, __LINE__ - 3);
        return 1;
    }

    /* push nums to heap */
    for(; arr_size > i; ++i)
    {
        HeapPush(test_heap, (void *)&nums[i]);
    }
    
    /* get heap size after pushes (should be 7) */
    actual_size = HeapSize(test_heap);

    /* check if size is indeed 7 */
    if(actual_size != expected_size)
    {
        HeapDestroy(test_heap);
        printf("\t%sLine %d - Expected size %ld, actual size %ld\n",
                                 RED, __LINE__ - 4, expected_size, actual_size);
        return 1;
    }

    HeapDestroy(test_heap);
    return 0;
}

static int TestCreateDestroy()
{
    heap_t *test_heap = HeapCreate(CompareHolders);

    HeapDestroy(test_heap);

    return 0;
}

int main()
{
    RUN_TEST(TestCreateDestroy);
    RUN_TEST(TestInsertSizeIsEmptySimple);
    RUN_TEST(TestInsertComplex); 
    RUN_TEST(TestPop);
    RUN_TEST(TestRemove);
    RUN_TEST(TestRemoveLeaf);
    RUN_TEST(TestSort);
    
    return 0;
}
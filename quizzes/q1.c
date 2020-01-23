#include <time.h> /* time_t */
#include <stdlib.h> /* malloc */
#include <stdio.h>  /* printf */
#include <unistd.h> /* sleep */

typedef struct advanced_array 
{
    int *nums;
    clock_t *times;
    size_t size;
    clock_t last_set;
    int set_all;
} adv_arr_t;

adv_arr_t *CreateArr(size_t size)
{
    clock_t now = clock();
    size_t i = 0;
    int *nums = NULL;
    time_t *times = NULL;
    adv_arr_t *arr = NULL;

    arr = (adv_arr_t *)malloc(sizeof(adv_arr_t *) * size);
    if(NULL == arr)
    {
        return NULL;
    }

    nums = (int *)malloc(sizeof(int) * size);
    if(NULL == nums)
    {
        free(arr);
        
        return NULL;
    }

    times = (time_t *)malloc(sizeof(clock_t) * size);
    if(NULL == times)
    {
        free(arr);
        free(nums);
    
        return NULL;
    }

    for(; i < size; ++i)
    {
        times[i] = now;
    }

    arr->last_set = now;
    arr->nums = nums;
    arr->size = size;
    arr->times = times;
    arr->set_all = 0;

    return arr;
}

void SetAll(int val, adv_arr_t *arr)
{
    arr->last_set = clock();
    arr->set_all = val;
}

void SetVal(int index, int val, adv_arr_t *arr)
{
    arr->nums[index] = val;
    arr->times[index] = clock();
}

int GetVal(int index, adv_arr_t *arr)
{  
    if(0 < (arr->last_set - arr->times[index]))
    {
        arr->nums[index] = arr->set_all;
    }

    arr->times[index] = clock();

    return arr->nums[index];
}

int main()
{
    adv_arr_t *arr = CreateArr(10);

    SetVal(0, 0, arr);
    SetVal(1, 1, arr);
    SetVal(2, 2, arr);

    SetAll(23, arr);

    SetVal(0, 10, arr);

    SetAll(400, arr);

    printf("%d\n", GetVal(0, arr));
    printf("%d\n", GetVal(1, arr));

    return 0;
}


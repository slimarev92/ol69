/**************************************************
* DS:          SORTING ALGORITHMS - RECURSIVE     *
* CODE BY:     SASHA LIMAREV                      *   
* REVIEWED BY: SOMEONE                            *                          
***************************************************/
#include <assert.h> /* assert */
#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <string.h> /* memmove */

#include "sort_rec.h" /* sort header file */

enum status MergeSortedArrs(int *arr1, int *arr2, size_t len1, size_t len2);

enum status MergeSort(int *arr, size_t len)
{
    size_t sub1_len = 0;
    size_t sub2_len = 0;

    assert(NULL != arr && 0 < len);

    if(1 >= len)
    {
        return OK;
    }

    sub1_len = len / 2;
    sub2_len = len - sub1_len;
    
    MergeSort(arr, sub1_len);
    MergeSort(arr + sub1_len, sub2_len);

    MergeSortedArrs(arr, (arr+sub1_len), sub1_len, sub2_len);
    
    return OK;
}

enum status MergeSortedArrs(int *arr1, int *arr2, size_t len1, size_t len2)
{
    size_t i = 0;
    int curr_num = 0;
    size_t iters = len1 + len2;
    int *runner1 = arr1;
    int *runner2 = arr2;

    int *arr_temp = (int *)malloc(sizeof(int) * (len1 + len2));
    if(NULL == arr_temp)
    {
        return NO_MEM;
    }
    
    for(; i < iters; ++i)
    {
        if(0 == len1)
        {
            curr_num = *runner2;
            ++runner2;
            --len2;
        }
        else if(0 == len2)
        {
            curr_num = *runner1;
            ++runner1;
            --len1;
        }
        else if(*runner1 < *runner2)
        {
            curr_num = *runner1;
            ++runner1;
            --len1;
        }
        else
        {
            curr_num = *runner2;
            ++runner2;
            --len2;
        }

        *(arr_temp + i) = curr_num;
    }

    memcpy(arr1, arr_temp, sizeof(int) * iters);
    free(arr_temp);

    return OK;
}
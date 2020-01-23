/**************************************************
* DS:          SORTING ALGORITHMS - ADVANCED      *
* CODE BY:     SASHA LIMAREV                      *   
* REVIEWED BY: ERAN SEGAL                         *                          
***************************************************/
#include <assert.h> /* assert */
#include <sys/types.h> /* ssize_t */
#include <stdlib.h> /* random */
#include <math.h> /* sqrt */

#include "sort_adv.h" /* sort (advanced) header file */

#define L_CHILD(index) (index * 2 + 1)
#define R_CHILD(index) (index * 2 + 2)

static void HeapifyArr(int arr[], size_t size, enum direction dir);
static size_t GetLargerIndex(int arr[], size_t, size_t, size_t, enum direction);
static void SwapIndices(int arr[], size_t i1, size_t i2);
static void HeapifyDown(int arr[], size_t size, size_t i, enum direction dir);
static int PlacePivotL(int arr[], size_t pivot, enum direction d);
static int PlacePivotR(int arr[], size_t n, size_t pivot, enum direction d);
static int BinSearchRec(const int arr[], int key, size_t from,size_t to, size_t *index);

int BinarySearch(const int arr[], int key, size_t size, size_t *index)
{
    assert(arr);
    assert(0 < size);
    assert(index);

    return BinSearchRec(arr, key, 0, size, index);
}

static int BinSearchRec(const int arr[], int key, size_t from, size_t to, size_t *index)
{
    size_t look_at = (from + to) / 2;

    if(arr[look_at] == key)
    {
        *index = look_at;
        return 1;
    }
    else if(look_at == from || look_at == to)
    {
        return 0;
    }
    else
    {
        if(arr[look_at] < key)
        {
            return BinSearchRec(arr, key, look_at, to, index);
        }
        else
        {
            return BinSearchRec(arr, key, 0, look_at, index);
        }
    }   
}

int JumpSearch(const int arr[], int key, size_t size, size_t *index)
{
    size_t curr = 0;
    size_t last_curr = 0; 
    size_t jump = sqrt(size);
    size_t temp = 0;

    assert(arr);
    assert(0 < size);
    assert(index);

    while(curr < size && arr[curr] < key)
    {
        last_curr = curr;
        curr += jump;  
    }

    temp = curr;
    curr = last_curr;
    last_curr = temp;

    for(; curr < last_curr && curr < size && arr[curr] != key; ++curr)
    {
        /* do nothing */
    }

    if(arr[curr] == key)
    {
        *index = curr;
        return 1;
    }
    else
    {        
        return 0;
    }

}

void QuickSort(int arr[], size_t size, enum direction dir)
{
    size_t pivot = 0;

    assert(arr);

    if(0 == size || 1 == size)
    {
        return;
    }

    pivot = rand() % size;

    pivot = PlacePivotL(arr, pivot, dir);
    pivot = PlacePivotR(arr, size, pivot, dir);

    QuickSort(arr, pivot, dir); 
    QuickSort(arr + pivot + 1, size - pivot - 1, dir);
}

static int PlacePivotR(int arr[], size_t size, size_t pivot, enum direction dir)
{
    size_t curr = size - 1;
    int cmp = 0;
    int temp = 0;

    while(pivot < curr)
    {
        cmp = (dir == ASC) ? arr[curr] < arr[pivot] : arr[curr] > arr[pivot];
        
        /* place arr[curr] to the right of pivot */
        if(1 == cmp)
        {
            temp = arr[pivot];
            arr[pivot] = arr[curr];
            arr[curr] = arr[pivot + 1];
            arr[pivot + 1] = temp;
            ++pivot;
        }
        else
        {
            --curr;
        }
    }

    return pivot;
}

static int PlacePivotL(int arr[], size_t pivot, enum direction dir)
{
    size_t curr = 0;
    int cmp = 0;
    int temp = 0;

    while(pivot > curr)
    {
        cmp = (dir == ASC) ? arr[curr] > arr[pivot] : arr[curr] < arr[pivot];
        
        /*place arr[curr] to the left of pivot */
        if(1 == cmp)
        {
            temp = arr[pivot]; 
            arr[pivot] = arr[curr];
            arr[curr] = arr[pivot - 1]; 
            arr[pivot - 1] = temp;
            --pivot;
        }
        else
        {
            ++curr;
        }
    }

    return pivot;
}

void HeapSort(int arr[], size_t size, enum direction dir)
{
    size_t curr_end = size - 1;

    assert(arr);
    assert(0 < size);

    HeapifyArr(arr, size, dir);

    for(; 0 < curr_end ; --curr_end)
    {
        SwapIndices(arr, 0, curr_end);
        HeapifyDown(arr, curr_end, 0, dir);
    }
}

static void HeapifyDown(int arr[], size_t size, size_t i, enum direction dir)
{
    size_t swap_index = 0;

    while(i <= (size / 2) - 1 )
    {
        swap_index = GetLargerIndex(arr, size, L_CHILD(i), R_CHILD(i), dir);
        swap_index = GetLargerIndex(arr, size, i, swap_index, dir);

        if(swap_index != i)
        {
            SwapIndices(arr, i, swap_index);
            i = swap_index;
        }
        else
        {
            break;
        }
    }
}

static void HeapifyArr(int arr[], size_t size, enum direction dir)
{
    size_t last_parent = (size / 2) - 1;
    ssize_t i = 0;

    (void)dir;

    for(i = last_parent; 0 <= i; --i)
    {
        HeapifyDown(arr, size, i, dir);
    }
}

static size_t GetLargerIndex(int arr[], size_t size, size_t left, 
                                             size_t right, enum direction dir)
{
    int cmp = 0;

    /*  only left child exists */
    if(left < size && right >= size)
    {
        return left;
    }
    /* both children exist */
    else
    {
        cmp = (ASC == dir) ? arr[right] >= arr[left] : arr[right] <= arr[left];

        return (1 == cmp) ? right : left;
    }  
}

static void SwapIndices(int arr[], size_t i1, size_t i2)
{
    int temp = arr[i1];
    
    arr[i1] = arr[i2];
    arr[i2] = temp;
}
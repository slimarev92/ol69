/**************************************
* DS:          SORTING ALGORITHMS     *
* CODE BY:     SASHA LIMAREV          *   
* REVIEWED BY: ERAN SEGAL             *                          
***************************************/
#include <assert.h> /* assert */
#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <string.h> /* memmove */

#include "../include/sort.h" /* sort header file */

#define EPIC_FAIL (-1)
#define GREAT_SUCCESS (0)
#define NIBBLE_SIZE (4)

typedef unsigned char uchar_t;

static void SwapByte(uchar_t *byte1, uchar_t *byte2);
static void Swap(void *first, void *second, size_t size);
static void RotateBytes(uchar_t *arr, size_t el_size, size_t arr_size);
static size_t HexKTN(const void *data, const void *args);

typedef struct info_holder
{
    size_t shift;
    key_to_num user_ktn;
    void *user_args;
} info_holder_t;

int RadixSort(void *base, size_t n_memb, size_t el_size,
					          key_to_num ktn, const void *args, size_t n_bytes)
{
    size_t num_iters = 0;
    size_t i = 0;    
    info_holder_t *holder = NULL;

    assert(NULL != base);
    assert(NULL != ktn);

    holder = (info_holder_t *)malloc(sizeof(info_holder_t));
    if(NULL == holder)
    {
        return EPIC_FAIL;
    }

    holder->shift = 0;
    holder->user_args = (void *)args;
    holder->user_ktn = ktn;

    num_iters = n_bytes * 2;

    for(; i < num_iters; ++i)
    {
        CountingSort(base, n_memb, el_size, HexKTN, (void *)holder, 16);
        holder->shift += NIBBLE_SIZE;
    }

    free(holder);

    return GREAT_SUCCESS;
}

int CountingSort(void *base, size_t n_memb, size_t el_size,
					            key_to_num ktn, const void *args, size_t range)
{
    char *base_copy = NULL;
    void *copy_from = NULL;
    void *copy_to = NULL;
    size_t *freq_table = NULL;
    size_t key = 0;
    size_t i = 0;

    assert(NULL != base);
    assert(NULL != ktn);

    base_copy = malloc(n_memb * el_size);
    if(NULL == base_copy)
    {
        return EPIC_FAIL;
    }

    freq_table = (size_t *)calloc(sizeof(size_t), range);
    if(NULL == freq_table)
    {
        free(base_copy);
        return EPIC_FAIL;
    }

    memcpy((void *)base_copy, base, n_memb * el_size);

    for(; i < n_memb; ++i)
    {
        key = ktn((void *)(base_copy + (i * el_size)), args);
        ++freq_table[key];
    }

    for(i = 1; i < range; ++i)
    {
        freq_table[i] += freq_table[i - 1];
    }

    for(i = 0; i < n_memb; ++i)
    {
        key = ktn((void *)(base_copy + ((n_memb - i  - 1) * el_size)), args);   
        --freq_table[key];
   
        copy_from = (void *)((char *)base + (freq_table[key] * el_size));
        copy_to = (void *)(base_copy + ((n_memb - i  - 1) * el_size));

        memcpy(copy_from, copy_to, el_size);
    }

    free(freq_table);
    free(base_copy);

    return GREAT_SUCCESS;
}

void SelectionSort(void *base, size_t n_memb, size_t size, cmp_f cmp)
{
    uchar_t *unsorted = NULL;
    uchar_t *smallest = NULL;
    uchar_t *end = NULL;
    uchar_t *curr = NULL;

    assert(NULL != base && NULL != cmp);
 
    end = (uchar_t *)base + (n_memb * size);

    for(unsorted = (uchar_t *)base; unsorted < end; unsorted += size)
    {            
        for(curr = unsorted, smallest = unsorted; curr < end; curr += size)
        {  
            if(0 > cmp((void *)(curr), (void *)(smallest)))
            {
                smallest = curr;
            }      
        }

        if(unsorted != smallest)
        {
            Swap(smallest, unsorted, size);
        }
    }
}

void InsertionSort(void *base, size_t n_memb, size_t size, cmp_f cmp)
{
    uchar_t *sorted = 0;
    uchar_t *unsorted = NULL;
    uchar_t *end = NULL;

    assert(NULL != base && NULL != cmp);

    end = (uchar_t *)base + (size * n_memb);

    for(unsorted = (uchar_t *)base; unsorted < end; unsorted += size)
    {             
        for(sorted = (uchar_t *)base; sorted <= unsorted; sorted += size)
        {
            if(0 > cmp((void *)(unsorted), (void *)(sorted)))
            {
                RotateBytes(sorted, size, (size_t)(unsorted - sorted) + size);
                break;
            }
        }
    }
}

void BubbleSort(void *base, size_t n_memb, size_t size, cmp_f cmp)
{
    int was_swap = 1;
    uchar_t *curr = NULL;
    uchar_t *next = NULL;
    uchar_t *end = NULL;

    assert(NULL != base && NULL != cmp);

    end = (uchar_t *)base + (n_memb * size);

    while(was_swap)
    {
        was_swap = 0; 

        for(curr = (uchar_t *)base; curr < end; curr += size)
        {
            next = curr + size;

            if(0 > cmp((void *)(next), (void *)(curr)))
            {
                was_swap = 1;
                Swap((void *)curr, (void *)next, size);
            }
        }    
    } 
}

static void RotateBytes(uchar_t *arr, size_t el_size, size_t arr_size)
{
    size_t i = 0;
    uchar_t *last_el = (uchar_t *)malloc(el_size);
    if(NULL == last_el)
    {
        return;
    }

    for(i = 0; i < el_size; ++i)
    {
        last_el[i] = arr[arr_size - el_size + i];
    }

    memmove((void *)((char *)arr + el_size), arr, arr_size - el_size);
    memcpy((void *)arr, last_el, el_size);

    free(last_el);
}

static size_t HexKTN(const void *data, const void *args)
{
    info_holder_t *holder = (info_holder_t *)args;    
    size_t user_key = 0;
    size_t shift = holder->shift;

    user_key = holder->user_ktn(data, holder->user_args);

    return (user_key >> shift) & 0xF; 
}

static void SwapByte(uchar_t *byte1, uchar_t *byte2)
{
    *byte2 = *byte1 ^ *byte2;
    *byte1 = *byte1 ^ *byte2;
    *byte2 = *byte1 ^ *byte2;
}

static void Swap(void *first, void *second, size_t size)
{
    size_t i = 0;

    for(; i < size; ++i)
    {
        SwapByte((uchar_t *)first + i, (uchar_t *)second + i);
    }
}

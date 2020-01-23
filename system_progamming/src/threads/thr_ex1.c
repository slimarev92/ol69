/*******************************************************
*                                                      *
*      .oooooo.   ooooo            .ooo    .ooooo.     *
*     d8P'  `Y8b  `888'          .88'     888' `Y88.   *
*    888      888  888          d88'      888    888   *
*    888      888  888         d888P"Ybo.  `Vbood888   *
*    888      888  888         Y88[   ]88       888'   *
*    `88b    d88'  888       o `Y88   88P     .88P'    *
*     `Y8bood8P'  o888ooooood8  `88bod8'    .oP'       *
*                                                      *
*         SYSTEM PROGRAMMING: THREADS EX 1             *
*         CODE BY:            SASHA LIMAREV            *
*         REVIEWED BY:        PROBABLY NOBODY          * 
*******************************************************/
#include <pthread.h> /* pthread_create */
#include <stdlib.h> /* malloc */
#include <stdio.h> /* printnf */
#include <assert.h> /* assert */
#include <unistd.h> /* sleep */

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[93m"
#define RESET_COLOR "\x1b[0m"

#define ARR_SIZE (100000)
#define COUNTER_MAX (10000)

#define RUN_TEST(test_name) \
    printf("\n%s[Calling: " #test_name "]\n%s", YELLOW, RESET_COLOR); \
    \
    if(0 == test_name()) \
    { \
        printf("%s[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \
    } \
    printf("%s\n", RESET_COLOR);

typedef struct
{
    int *nums;
    int index;
} info_t;

static void *FillIndex(void *args);
static int FillArray(size_t arr_size);
static int InitArrs(pthread_t **thread_arr, int **nums_arr, info_t **info,
                                                                   size_t size);
static int TestFill();

static int InitArrs(pthread_t **thread_arr, int **nums_arr, info_t **info,
                                                                    size_t size)
{
    *nums_arr = (int *)malloc(sizeof(int) * size);
    if(NULL == nums_arr)
    {
        return 1;
    }

    *thread_arr = (pthread_t *)malloc(sizeof(pthread_t) * size);
    if(NULL == thread_arr)
    {
        free(*nums_arr);
        return 1;
    }

    *info = (info_t *)malloc(sizeof(info_t) * size);
    if(NULL == *info)
    {
        free(*thread_arr);
        free(*nums_arr);
        
        return 1;
    }

    return 0;
}

static void *FillIndex(void *args)
{
    info_t *info = NULL;

    assert(args);

    info = (info_t *)args;

    (info->nums)[info->index] = (int)info->index + 1;

    return NULL;
}

static int FillArray(size_t arr_size)
{
    size_t i = 0;
    size_t join_from = 0, join_to = 0;
    int expected_num = 0;
    int *nums_arr = NULL;
    pthread_t *thread_arr = NULL;
    info_t *info_arr = NULL;
    size_t join_counter = 0;

    /* create and init empty arrays */
    if(0 != InitArrs(&thread_arr, &nums_arr, &info_arr , arr_size))
    {
        return 1;
    }

    /* fill array, then sleep */
    for(; i < arr_size; ++i, ++join_counter)
    {
        (info_arr + i)->nums = nums_arr;
        (info_arr + i)->index = i;
        pthread_create((thread_arr + i), NULL, FillIndex, (info_arr + i));

        /* join last 10,000 threads */
        if(COUNTER_MAX == join_counter || i == arr_size - 1)
        {
            join_from = i - COUNTER_MAX;
            join_to = i + 1;

            for(; join_from < join_to; ++join_from)
            {
                pthread_join(thread_arr[join_from], NULL);  
            }

            join_counter = 0;
        }
    }

    /* check if array is properly filled */
    for(i = 0; i < arr_size; ++i)
    {
        expected_num = (int)i + 1;

        if (expected_num != nums_arr[i])
        {
            free(info_arr);
            free(thread_arr);
            free(nums_arr);

            printf("\t%sLine %d: Expected num %d, actual num %d (index %lu)\n",
                            RED, __LINE__ - 4, expected_num , nums_arr[i], i);
            return 1;
        }
    }
    
    free(info_arr);
    free(thread_arr);
    free(nums_arr);

    return 0;
}

static int TestFill()
{
    return FillArray(ARR_SIZE);
}

int main()
{
  
    RUN_TEST(TestFill);

    return 0;
}


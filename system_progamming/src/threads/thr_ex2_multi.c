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
*   SYSTEM PROGRAMMING: THREADS EX 2 - MUTLTI THREAD   *
*   CODE BY:            SASHA LIMAREV                  *
*   REVIEWED BY:        PROBABLY NOBODY                * 
*******************************************************/
#include <assert.h> /* assert */
#include <stdio.h> /* printf */
#include <stdlib.h> /* atol */
#include <pthread.h> /* pthread_create */

#define MAX_THREADS (32000)

typedef struct 
{
    size_t num;
    size_t sod;
    size_t from;
    size_t to;
} sum_info_t;

static size_t g_total_sod = 0;

void *SumOfDevisors(void *args)
{
    size_t from = 0;
    sum_info_t *s_info = NULL;

    assert(args);

    s_info = (sum_info_t *)args;

    from = s_info->from;
    
    for(; from <= s_info->to; ++from)
    {
        if(0 == s_info->num % from)
        {
            s_info->sod += from;
        }
    }

/*    printf("Curr sod is: %lu\n", s_info->sod);*/

    return NULL;
}

int main(int argc, char **argv)
{
    size_t big_num = 0xFFF000;
    sum_info_t *info_arr = NULL;
    pthread_t *thread_arr = NULL;
    size_t n_threads = 0;
    size_t range = 0;
    size_t total_sod = 0;
    size_t i = 0, j = 1;

    (void)argc;
    assert(1 < argc);

    n_threads = (size_t)atol(argv[1]);
    range = big_num / n_threads;
    
    printf("Big num is: %lu\n", big_num);
    printf("Number of threads: %lu\n", n_threads);
    printf("Range is: %lu\n", range);
    
    info_arr = (sum_info_t *)malloc(sizeof(sum_info_t) * n_threads);
    if(NULL == info_arr)
    {
        return 1;
    }

    thread_arr = (pthread_t *)malloc(sizeof(pthread_t) * n_threads);
    if(NULL == thread_arr)
    {
        free(info_arr);

        return 1;
    }

    for(i = 0; i < n_threads; ++i)
    {
        info_arr[i].num = big_num;
        info_arr[i].sod = 0;
        info_arr[i].from = j;
        info_arr[i].to = j + range - 1; 

        j += range;
        if(0 !=
           pthread_create(&thread_arr[i], NULL, SumOfDevisors,  info_arr + i))
        {
            perror("pthread_create failed! \n");
        }
    }

    for(i = 0; i < n_threads; ++i)
    {
        pthread_join(thread_arr[i], NULL);
        
        g_total_sod +=  info_arr[i].sod;
    }

    printf("-----------------------\n");

    printf("Total SOD is: %lu\n", total_sod);
    
    free(info_arr);
    free(thread_arr);

    return 0;
}

/* 

Big num is: 16773120
Number of threads: 10
Range is: 1677312
-----------------------
Total SOD is: 0

real    0m0.046s
user    0m0.165s
sys     0m0.000s
-----------------------

Big num is: 16773120
Number of threads: 112
Range is: 149760
-----------------------
Total SOD is: 0

real    0m0.044s
user    0m0.165s
sys     0m0.000s
-----------------------

Big num is: 16773120
Number of threads: 224
Range is: 74880
-----------------------
Total SOD is: 0

real    0m0.046s
user    0m0.157s
sys 0m0.004s
-----------------------

Big num is: 16773120
Number of threads: 1260
Range is: 13312
-----------------------
Total SOD is: 0

real    0m0.073s
user    0m0.163s
sys 0m0.049s
-----------------------

Big num is: 16773120
Number of threads: 14560
Range is: 1152
-----------------------
Total SOD is: 0

real    0m0.365s
user    0m0.234s
sys 0m0.432s
-----------------------
*/
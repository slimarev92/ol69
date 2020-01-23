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
*         SYSTEM PROGRAMMING: PRODUCER CONSUMER 2      *
*         CODE BY:            SASHA LIMAREV            *
*         REVIEWED BY:        YOAV KLEIN               * 
*******************************************************/
#include <pthread.h> /* pthread_create */
#include <stdio.h> /* printf */
#include <unistd.h> /* sleep */
#include <stdlib.h> /* malloc */

#include "dll.h" /* DLLPush */

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[93m"
#define RESET_COLOR "\x1b[0m"
#define BLUE "\x1b[34m"
#define CYAN "\x1b[36m"

#define N_CONSUME (2)
#define N_PRODUCE (5)
#define ALWAYS (1)

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

static int Init(pthread_t **producers, pthread_t **consumers);
static void *Consumer();
static void *Producer();

static pthread_mutex_t g_q_lock = PTHREAD_MUTEX_INITIALIZER;
static volatile size_t g_produced_counter = 0;
static volatile size_t g_n_tasks = 0;
static dll_t *g_q = NULL;

static void *Producer(void *args)
{
    size_t thread_num = (size_t)args;

    while(ALWAYS)
    {
        ++g_produced_counter;

        sleep(1);

        pthread_mutex_lock(&g_q_lock);

        printf("%s\t%lu: Producing task %lu\n", BLUE,
                                                thread_num,
                                                g_produced_counter);

        DLLPushBack(g_q, (void *)g_produced_counter);
        ++g_n_tasks;

        pthread_mutex_unlock(&g_q_lock);
    }

    return NULL;
}

static void *Consumer(void *args)
{
    size_t curr_num = 0;
    int can_consume = 0;
    size_t thread_num = (size_t)args;

    while(ALWAYS)
    {
        pthread_mutex_lock(&g_q_lock);

        if(0 < g_n_tasks)
        {
            curr_num = (size_t)DLLGetData(DLLBegin(g_q));

            DLLPopFront(g_q);
            --g_n_tasks;
            can_consume = 1;
        }

        pthread_mutex_unlock(&g_q_lock);

        if(can_consume)
        {
            printf("%s\t%lu: Consuming task %lu\n", CYAN, thread_num, curr_num);
        }
        else
        {
            printf("%s\t%lu: Didn't consume task\n", CYAN, thread_num);
        }

        sleep(1);
    }

    return NULL;
}

static int Init(pthread_t **producers, pthread_t **consumers)
{
    g_q = DLLCreate();
    
    *producers = (pthread_t *)malloc(sizeof(pthread_t) * N_PRODUCE);
    *consumers = (pthread_t *)malloc(sizeof(pthread_t) * N_CONSUME);

    return (NULL == g_q)        || 
           (NULL == *producers) || 
           (NULL == *consumers) ? 1 : 0;
}

int Run()
{
    size_t i = 0, j = 0;
    pthread_t *consumers = NULL;
    pthread_t *producers = NULL;

    if(0 != Init(&producers, &consumers))
    {
        DLLDestroy(g_q);
        free(producers);
        free(consumers);
        return 1;
    }

    while(N_CONSUME > i || N_PRODUCE > j)
    {
        if(N_CONSUME > i)
        {
            printf("%s\tCreating consumer %lu\n", GREEN, i);
            pthread_create(consumers + i, NULL, Consumer, (void *)i);
            ++i;
        }

        if(N_PRODUCE > j)
        {
            printf("\t%sCreating producer %lu\n", YELLOW, j);
            pthread_create(producers + j, NULL, Producer, (void *)j);
            ++j;
        }
    }

    for(i = 0; i < N_CONSUME; ++i)
    {
        pthread_join(*(consumers + i), NULL);
    }

    for(i = 0; i < N_PRODUCE; ++i)
    {
        pthread_join(*(producers + i), NULL);
    }

    free(producers);
    free(consumers);
    DLLDestroy(g_q);

    return 0;
}

int main()
{
    return Run();
}
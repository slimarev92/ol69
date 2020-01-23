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
*         SYSTEM PROGRAMMING: PRODUCER CONSUMER 3      *
*         CODE BY:            SASHA LIMAREV            *
*         REVIEWED BY:        YOAV KLEIN               * 
*******************************************************/
#include <pthread.h> /* pthread_create */
#include <semaphore.h> /* post */
#include <stdio.h> /* printf */
#include <unistd.h> /* sleep */
#include <stdlib.h> /* malloc */

#include "dll.h" /* DLLPush */

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[93m"
#define BLUE "\x1b[34m"
#define CYAN "\x1b[36m"

#define N_CONSUME (2)
#define N_PRODUCE (5)
#define ALWAYS (1)

static int Run();
static int Init(pthread_t **producers, pthread_t **consumers);
static void *Consumer();
static void *Producer();

static pthread_mutex_t g_q_lock = PTHREAD_MUTEX_INITIALIZER;
static sem_t g_n_consumeable;
static size_t g_counter = 0;
static dll_t *g_task_q = NULL;

static void *Producer(void *args)
{
    size_t thread_num = (size_t)args;

    while(ALWAYS)
    {
        ++g_counter;
        printf("%s\t%lu: Producing task %lu\n", BLUE, thread_num, g_counter);

        sleep(1);

        pthread_mutex_lock(&g_q_lock);

        DLLPushBack(g_task_q, (void *)g_counter);
        sem_post(&g_n_consumeable);

        pthread_mutex_unlock(&g_q_lock);
    }

    return NULL;
}

static void *Consumer(void *args)
{
    size_t curr_num = 0;
    size_t thread_num = (size_t)args;

    while(ALWAYS)
    {
        sem_wait(&g_n_consumeable);

        pthread_mutex_lock(&g_q_lock);

        curr_num = (size_t)DLLGetData(DLLBegin(g_task_q));
        DLLPopFront(g_task_q);

        pthread_mutex_unlock(&g_q_lock);

        printf("%s\t%lu: Consuming task %lu\n", CYAN, thread_num, curr_num);
    }

    return NULL;
}

static int Init(pthread_t **producers, pthread_t **consumers)
{
    g_task_q = DLLCreate();
   
    *producers = (pthread_t *)malloc(sizeof(pthread_t) * N_PRODUCE);
    *consumers = (pthread_t *)malloc(sizeof(pthread_t) * N_CONSUME);

    return (NULL == g_task_q)                      || 
           (NULL == *producers)                    || 
           (0 != sem_init(&g_n_consumeable, 0, 0)) ||
           (NULL == *consumers) ? 1 : 0;
}

static int Run()
{
    size_t i = 0, j = 0;
    pthread_t *consumers = NULL, *producers = NULL;
    size_t is_fail = Init(&producers, &consumers);

    while((N_CONSUME > i || N_PRODUCE > j) && !is_fail)
    {
        if(N_CONSUME > i)
        {
            if(0 != pthread_create(consumers + i, NULL, Consumer, (void *)i))
            {
                is_fail = 1;
            }
            else
            {
                printf("%s\tCreated consumer %lu\n", GREEN, i);
                ++i;
            }
        }

        if(N_PRODUCE > j)
        {
            if(0 != pthread_create(producers + j, NULL, Producer, (void *)j))
            {
                is_fail = 1;
            }
            else
            {
                printf("\t%sCreated producer %lu\n", YELLOW, j);
                ++j;
            }
        }
    }

    for(i = 0; i < N_CONSUME && !is_fail; ++i)
    {
        pthread_join(*(consumers + i), NULL);
    }

    for(i = 0; i < N_PRODUCE && !is_fail; ++i)
    {
        pthread_join(*(producers + i), NULL);
    }

    sem_destroy(&g_n_consumeable);
    free(producers);
    free(consumers);
    DLLDestroy(g_task_q);

    return is_fail;
}

int main()
{
    return Run();
}
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
*         SYSTEM PROGRAMMING: PRODUCER CONSUMER 4      *
*         CODE BY:            SASHA LIMAREV            *
*         REVIEWED BY:        YOAV KLEIN               * 
*******************************************************/
#include <pthread.h> /* pthread_create */
#include <semaphore.h> /* post */
#include <stdio.h> /* printf */
#include <unistd.h> /* sleep */
#include <stdlib.h> /* malloc */

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[93m"
#define BLUE "\x1b[34m"
#define CYAN "\x1b[36m"

#define Q_SIZE (20)
#define N_CONSUME (20)
#define N_PRODUCE (20)
#define ALWAYS (1)

static int Init(pthread_t **producers, pthread_t **consumers);
static int Run();
static int CreateThreads(pthread_t **producers, pthread_t **consumers);
static void *Consumer();
static void *Producer();

static pthread_mutex_t g_q_lock = PTHREAD_MUTEX_INITIALIZER;
static sem_t g_n_consumable;
static sem_t g_n_producible;
static size_t g_counter = 0;
static size_t g_curr_produce = 0;
static size_t g_curr_consume = 0;
static size_t *g_task_q = NULL;

static void *Producer(void *args)
{
    size_t thread_num = (size_t)args;

    while(ALWAYS)
    {
        ++g_counter;

        sem_wait(&g_n_producible);

        pthread_mutex_lock(&g_q_lock);

        
        g_task_q[g_curr_produce] = g_counter;
        g_curr_produce = (g_curr_produce + 1) % Q_SIZE;

        sem_post(&g_n_consumable);

        pthread_mutex_unlock(&g_q_lock);

        /* not thread safe - used for visual clarity */
        printf("%s\t%lu: Adding task %lu\n", BLUE, thread_num, g_counter);
    }

    return NULL;
}

static void *Consumer(void *args)
{
    size_t curr_num = 0;
    size_t thread_num = (size_t)args;

    while(ALWAYS)
    {
        sem_wait(&g_n_consumable);

        sleep(1);

        pthread_mutex_lock(&g_q_lock);

        curr_num = g_task_q[g_curr_consume];
        g_curr_consume = (g_curr_consume + 1) % Q_SIZE;
        sem_post(&g_n_producible);

        pthread_mutex_unlock(&g_q_lock);

        /* not thread safe - used for visual clarityqcq */
        printf("%s\t%lu: Consuming task %lu\n", CYAN, thread_num, curr_num);
    }

    return NULL;
}

static int Init(pthread_t **producers, pthread_t **consumers)
{
    g_task_q = (size_t *)malloc(sizeof(size_t) * Q_SIZE);
    *producers = (pthread_t *)malloc(sizeof(pthread_t) * N_PRODUCE);
    *consumers = (pthread_t *)malloc(sizeof(pthread_t) * N_CONSUME);

    return (NULL == g_task_q)                           || 
           (NULL == *producers)                         || 
           (0 != sem_init(&g_n_consumable, 0, 0))       ||
           (0 != sem_init(&g_n_producible, 0, Q_SIZE))  ||
           (NULL == *consumers) ? 1 : 0;
}

static int CreateThreads(pthread_t **producers, pthread_t **consumers)
{
    size_t i = 0, j = 0;

    while(N_CONSUME > i || N_PRODUCE > j)
    {
        if(N_CONSUME > i)
        {
            if(0 != pthread_create(*consumers + i, NULL, Consumer, (void *)i))
            {
                return 1;
            }
            else
            {
                printf("%s\tCreated consumer %lu\n", GREEN, i);
                ++i;
            }
        }

        if(N_PRODUCE > j)
        {
            if(0 != pthread_create(*producers + j, NULL, Producer, (void *)j))
            {
                return 1;
            }
            else
            {
                printf("\t%sCreated producer %lu\n", YELLOW, j);
                ++j;
            }
        }
    }

    return 0;
}

static int Run()
{
    size_t i = 0;
    pthread_t *consumers = NULL, *producers = NULL;
    size_t is_fail = Init(&producers, &consumers);

    if(!is_fail)
    {
        is_fail = CreateThreads(&producers, &consumers);
    }

    for(i = 0; i < N_CONSUME && !is_fail; ++i)
    {
        pthread_join(*(consumers + i), NULL);
    }

    for(i = 0; i < N_PRODUCE && !is_fail; ++i)
    {
        pthread_join(*(producers + i), NULL);
    }

    sem_destroy(&g_n_producible);
    sem_destroy(&g_n_consumable);
    free(producers);
    free(consumers);
    free(g_task_q);

    return is_fail;
}

int main()
{
    return Run();
}
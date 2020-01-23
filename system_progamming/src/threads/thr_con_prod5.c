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
*         SYSTEM PROGRAMMING: PRODUCER CONSUMER 5      *
*         CODE BY:            SASHA LIMAREV            *
*         REVIEWED BY:        YOAV KLEIN               * 
*******************************************************/
#include <pthread.h> /* pthread_create */
#include <semaphore.h> /* post */
#include <stdio.h> /* printf */
#include <unistd.h> /* sleep */
#include <stdlib.h> /* malloc */

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

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[93m"
#define BLUE "\x1b[34m"
#define CYAN "\x1b[36m"
#define RESET_COLOR "\x1b[0m"

#define ARR_SIZE (10)
#define N_CONSUME (55)
#define ALWAYS (1)

static int Init(pthread_t **producer, pthread_t **consumers);
static int Run();
static void *Consumer();
static void *Producer();
static int CreateThreads(pthread_t **producer, pthread_t **consumers);

static pthread_mutex_t g_arr_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_read_cond = PTHREAD_COND_INITIALIZER;
static sem_t g_consumers_ready;
static size_t *g_arr = NULL;
static size_t g_is_fail = 0;
static volatile size_t g_can_read = 0;

static void *Producer(void *args)
{
    int i = 0;
    int to_wait = 0;

    (void)args;

    while(ALWAYS && !g_is_fail)
    {
        for(to_wait = 0; N_CONSUME > to_wait; ++to_wait)
        {
            sem_wait(&g_consumers_ready);
        }

		g_can_read = 0;

        pthread_mutex_lock(&g_arr_lock);

		/* output might be garbled due to multithreading */
        printf("%s------------------------------\n", YELLOW);

        for(i = 0; i < ARR_SIZE; ++i)
        {	
			/* output might be garbled due to multithreading */
            printf("%s\tAdding %d\n", BLUE, i);
            g_arr[i] = i;
        }

		g_can_read = 1;
        pthread_cond_broadcast(&g_read_cond);
        
        pthread_mutex_unlock(&g_arr_lock);

        /* to easily read the output, not for synching */
        sleep(2);
    }

    return NULL;
}

static void *Consumer(void *args)
{
    size_t i = 0;
    size_t nums_cpy[ARR_SIZE] = {0};

    (void)args;

    while(ALWAYS && !g_is_fail)
    {
        /* fill local array with 0 - for testing */
        for(i = 0; ARR_SIZE > i; ++i)
        {
            nums_cpy[i] = 0;
        }

        pthread_mutex_lock(&g_arr_lock);

        sem_post(&g_consumers_ready);

		while(!g_can_read)
		{
        	pthread_cond_wait(&g_read_cond, &g_arr_lock);
		}

        for(i = 0; ARR_SIZE > i; ++i)
        {
            nums_cpy[i] = g_arr[i];
        }

        pthread_mutex_unlock(&g_arr_lock);

        /* check if array was filled properly */
        for(i = 0; ARR_SIZE > i; ++i)
        {
            if(i != nums_cpy[i])
            {
                g_is_fail = 1;
            }
        }
    }

    return NULL;
}

static int Init(pthread_t **producer, pthread_t **consumers)
{
    g_arr = (size_t *)malloc(sizeof(size_t) * ARR_SIZE);
    *producer = (pthread_t *)malloc(sizeof(pthread_t));
    *consumers = (pthread_t *)malloc(sizeof(pthread_t) * N_CONSUME);

    return (NULL == g_arr)                         || 
           (NULL == *producer)                     || 
           (0 != sem_init(&g_consumers_ready, 0, 0))   ||
           (NULL == *consumers) ? 1 : 0;
}

static int CreateThreads(pthread_t **producer, pthread_t **consumers)
{
    size_t i = 0;

    if(0 != pthread_create(*producer, NULL, Producer, NULL))
    {
        return 1;
    }
    else
    {
        /* output might be garbled due to multithreading */
        printf("\t%sCreated producer\n", YELLOW);
    }

    while(N_CONSUME > i)
    {
        if(0 != pthread_create(*consumers + i, NULL, Consumer, (void *)i))
        {
            return 1;
        }
        else
        {
            /* output might be garbled due to multithreading */
            printf("%s\tCreated consumer %lu\n", GREEN, i);
            ++i;
        }
    }

    return 0;
}

static int Run()
{
    size_t i = 0;
    pthread_t *consumers = NULL, *producer = NULL;
    
    g_is_fail = Init(&producer, &consumers);

    if(!g_is_fail)
    {
        g_is_fail = CreateThreads(&producer, &consumers);
    }

    for(i = 0; i < N_CONSUME && !g_is_fail; ++i)
    {
        pthread_join(*(consumers + i), NULL);
    }

    pthread_join(*producer, NULL);

    pthread_cond_destroy(&g_read_cond);
    pthread_mutex_destroy(&g_arr_lock);
    sem_destroy(&g_consumers_ready);
    free(producer);
    free(consumers);
    free(g_arr);

    return g_is_fail;
}

int main()
{
    RUN_TEST(Run);

    return 0;
}
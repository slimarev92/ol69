/***************************************
* DATA STRUCTURES: SCHEDULER           *
* CODE BY:         SASHA LIMAREV       *   
* REVIEWED BY:     BEN IZAKSON         *                          
****************************************/ 
#include <time.h> /* time_t */
#include <stdlib.h> /* malloc */
#include <unistd.h> /* sleep */
#include <errno.h> /* errno */
#include <assert.h> /* assert */

#include "../include/scheduler.h" /* scheduler library header */
#include "../include/task.h" /* task library header */
#include "../include/pq.h" /* PQ library header */

struct scheduler
{
	pq_t *task_queue;
	int should_stop;	
};

static int IsNewBefore(const void *exist, const void *new, const void *args);
static int IsSameDestr(const void *data, const void *key, const void *args);
static unsigned int SecondsToExecution(task_t *task);

int ScdRemove(scd_t *scd, unid_t task_uid)
{
    assert(NULL != scd && !UIDIsBad(task_uid));

    return PQErase(scd->task_queue, (void *)&task_uid, IsSameDestr);
}

size_t ScdRun(scd_t *scd)
{
    task_t *top_task = NULL;
    int execute_retval = 0;
    int time_to_sleep = 0;
    
    assert(NULL != scd);

    scd->should_stop = 0;

    while(!scd->should_stop && !ScdIsEmpty(scd))
    {   
        top_task = PQPeek(scd-> task_queue);

        time_to_sleep = SecondsToExecution(top_task);

        while(0 < time_to_sleep)
        {
            time_to_sleep = sleep(time_to_sleep);
        }
        
        execute_retval = TaskExecute(top_task);     

        if(0 != execute_retval)
        {
            if(0 != PQEnqueue(scd->task_queue, (void *)top_task))
            {
                errno = ENOMEM;

                return ScdSize(scd);               
            }
        }
        else
        {
            TaskDestroy(top_task);
        }

       PQDequeue(scd->task_queue);
    }

    return ScdSize(scd);
}

void ScdStop(scd_t *scd)
{
    assert(NULL != scd);

    scd->should_stop = 1;
}

unid_t ScdAdd(scd_t *scd, time_t interval, func_t func, void *params)
{
    task_t *new_task = NULL;

    assert(NULL != scd && NULL != func);   

    new_task = TaskCreate(interval, func, params);
    if(NULL == new_task)
    {
        return bad_uid;
    }

    if(0 != PQEnqueue(scd->task_queue, (void *)new_task))
    {
        TaskDestroy(new_task);

        return bad_uid;
    };
   
    return TaskGetUID(new_task);
}

size_t ScdSize(const scd_t *scd)
{
    assert(NULL != scd);

    return PQSize(scd->task_queue);
}

int ScdIsEmpty(const scd_t *scd)
{
    assert(NULL != scd);

    return PQIsEmpty(scd->task_queue);
}

void ScdDestroy(scd_t *scd)
{
    pq_t *task_queue = NULL;

    assert(NULL != scd);

    task_queue = scd->task_queue;

    while(!PQIsEmpty(task_queue))
    {
        free(PQPeek(task_queue));
        PQDequeue(task_queue);
    }

    PQDestroy(scd->task_queue);
    free(scd);    
}

scd_t *ScdCreate()
{
    pq_t *new_pq = NULL;

    scd_t *new_scd = (scd_t *)malloc(sizeof(scd_t));
    if(NULL == new_scd)
    {
        return NULL;
    }

    new_pq = PQCreate(IsNewBefore);
    if(NULL == new_pq)
    {
        free(new_scd);
        return NULL;
    }

    new_scd->task_queue = new_pq;
    new_scd->should_stop = 0;

    return new_scd;
}

static unsigned int SecondsToExecution(task_t *task)
{
     int time_until_execution = 0;
    
    assert(NULL != task);

    time_until_execution = difftime(TaskGetTime(task), time(NULL));

    return 0 >= time_until_execution ? 0 : time_until_execution;
}

static int IsNewBefore(const void *exist, const void *new, const void *args)
{
    task_t *exist_task = (task_t *)exist;
    task_t *new_task = (task_t *)new;
    
    (void)args;

    return TaskGetTime(new_task) < TaskGetTime(exist_task);
}

static int IsSameDestr(const void *data, const void *key, const void *args)
{
    task_t *curr = (task_t *)data;
    unid_t key_id = *(unid_t *)key;
    int is_same = UIDIsSame(TaskGetUID(curr), key_id);
 
    (void)args;
   
    if(is_same)
    {
        TaskDestroy(curr);
    }

    return is_same;
}

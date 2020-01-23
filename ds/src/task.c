/***************************************
* DATA STRUCTURES: SCHEDULER - TASK    *
* CODE BY:         SASHA LIMAREV       *   
* REVIEWED BY:     BEN IZAKSON         *                          
****************************************/ 
#include <time.h> /* time_t */
#include <stdlib.h> /* malloc */
#include "../include/task.h" /* task library header */

struct task
{
	func_t func;
	void *params;
	unid_t uid;
	size_t interval;
	time_t time_to_run;
};

void UpdateTimeToRun(task_t *task);

int TaskExecute(task_t *task) /* should take into account old task */
{
    long ttr = difftime(TaskGetTime(task), time(NULL));
    
    long func_retval = ttr == 0 ? task->func(task->params) : 0;

    if(0 > func_retval)
    {
        return 0;
    }
    else
    {
        task->interval = (0 == func_retval) ? 
                        task->interval : (size_t)func_retval;
        UpdateTimeToRun(task);

        return 1;
    }  
}

unid_t TaskGetUID(const task_t *task)
{
    return task->uid;
}

time_t TaskGetTime(const task_t *task)
{
    return task->time_to_run;
}

void TaskDestroy(task_t *task)
{
    free(task);
}

task_t *TaskCreate(size_t interval, func_t func, void *params)
{
    task_t *new_task = (task_t *)malloc(sizeof(task_t));
    if(NULL != new_task)
    {
        new_task->func = func;
        new_task->params = params;
        new_task->interval = interval;
        new_task->uid =  UIDCreate();
        UpdateTimeToRun(new_task);
    }
  
    return new_task;
}

void UpdateTimeToRun(task_t *task)
{
    task->time_to_run = time(NULL) + task->interval;
}

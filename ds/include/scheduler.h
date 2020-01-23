#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "uid.h"

#ifndef FUNC_T
#define FUNC_T
typedef long(*func_t)(void *params); /* returns negative number for remove task, 0 for no change in inverval or positive number for new inverval  */
#endif

typedef struct scheduler scd_t;

scd_t *ScdCreate();
void ScdDestroy(scd_t *scheduler);
unid_t ScdAdd(scd_t *scheduler, time_t interval, func_t func, void *params); 
int ScdRemove(scd_t *scheduler, unid_t task_uid); /* returns 0 on success or 1 on failure (uid not found)*/ 
size_t ScdRun(scd_t *scheduler); /* returns number of remaining tasks in the queue */
void ScdStop(scd_t *scheduler);
size_t ScdSize(const scd_t *scheduler);
int ScdIsEmpty(const scd_t *scheduler);

#endif



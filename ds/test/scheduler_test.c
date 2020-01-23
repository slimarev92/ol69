#include <stdio.h> /* printf */
#include <time.h> /* time */
#include <unistd.h> /* sleep */

#include "../include/scheduler.h" /*  scheduler header file */

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET_COLOR "\x1b[0m"
#define YELLOW "\x1b[93m"
#define BLUE "\x1b[36m"
#define PURPLE "\x1b[35m"
#define ORANGE "\x1b[33m"
#define CYAN "\x1b[34m"

#define RUN_TEST(test_name) \
    printf("\n%s[Calling: " #test_name "]\n%s", YELLOW, RESET_COLOR); \
    ++g_num_of_tests; \
    \
    if(0 == test_name()) \
    { \
        ++g_successful_tests; \
        printf("%s[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \
    } \
    printf("%s\n", RESET_COLOR);

static int g_num_of_tests = 0;
static int g_successful_tests = 0;
static unid_t task_uid;

void SetColor(size_t interval)
{
  switch(interval)
    {
        case 1:   
            printf("%s", ORANGE);
            break;
        case 5:
            printf("%s", CYAN);
            break;
        case 4:
            printf("%s", PURPLE);
        break;
        case 10:
            printf("%s", BLUE);
        break;
    }
}

long DisplayTime(void *params)
{
    size_t interval = *((size_t *)params);
    time_t curr_time = time(NULL);

    SetColor(interval);

    printf("    Interval: %.2ld Time: %s", interval ,ctime(&curr_time));
    printf("%s", RESET_COLOR);

    return (long)interval;
}

long SimpleTask(void *params)
{
    (void)params;
    printf("%s", BLUE);
    printf("    Performing a simple task\n");
    printf("%s", RESET_COLOR);

    return 1;
}

long AddTasks(void *params)
{
    scd_t *scd = (scd_t *)params;

    ScdAdd(scd , 1, SimpleTask, NULL);
    ScdAdd(scd , 2, SimpleTask, NULL);
    ScdAdd(scd , 3, SimpleTask, NULL);

    return -10;
}

long RemoveTask(void *params)
{
    scd_t *scd = (scd_t *)params;

    printf("    Removing task\n");

    ScdRemove(scd, task_uid);

    return -10;
}

long StopScd(void *params)
{
    scd_t *scd = (scd_t *)params;
    printf("%s", RESET_COLOR);
   
    ScdStop(scd);
    printf("    Stopping scheduler\n");

    return 15;     
}

long StopFile(void *params)
{
    scd_t *scd = (scd_t *)params;
    FILE *stop_file = fopen("sched.stop", "r");
    if(NULL != stop_file)
    {       
        printf("%s", RESET_COLOR);
        ScdStop(scd);
        fclose(stop_file);
        remove("sched.stop");
        printf("    Stopping scheduler\n");
    }
    
    return 1;     
}

long CreateStopFile(void *params)
{
    FILE *stop_file = fopen("sched.stop", "w");
    
    (void)params;  
    printf("%s", RESET_COLOR);
    printf("    Creating stop file\n");
    fclose(stop_file);

    return -1;     
}

int TestCreateDestroy()
{
    scd_t *test_scd = ScdCreate();

    ScdDestroy(test_scd);

    return 0;
}

int TestIsEmptySize()
{
    size_t test_interval = 3;
    size_t expected_size = 4;
    size_t actual_size = 0;
    scd_t *test_scd = ScdCreate();

    /* scd should be empty at this point */
    if(1 != ScdIsEmpty(test_scd)) 
    {
        ScdDestroy(test_scd);
        printf("    %sLine %d - Scd should be empty\n",
               RED, __LINE__ - 4);
        return 1;
    }

    /* adding four tasks */
    ScdAdd(test_scd , test_interval, DisplayTime, &test_interval);
    ScdAdd(test_scd , test_interval, DisplayTime, &test_interval);
    ScdAdd(test_scd , test_interval, DisplayTime, &test_interval);
    ScdAdd(test_scd , test_interval, DisplayTime, &test_interval);

    actual_size = ScdSize(test_scd);

    /* size should be 4 */
    if(actual_size != expected_size)
    {
        ScdDestroy(test_scd);
        printf("    %sLine %d - Expected Size: %ld, Actual Size: %ld\n",
               RED, __LINE__ - 4, expected_size, actual_size);
        return 1;
    }

    ScdDestroy(test_scd);
    return 0;
}

int TestRemove()
{
    scd_t *test_scd = ScdCreate();
    size_t test_interval = 3;
    int i = 0;
    unid_t uids[4];

    /* adding four tasks */
    uids[0] = ScdAdd(test_scd , test_interval, DisplayTime, &test_interval);
    uids[1] = ScdAdd(test_scd , test_interval, DisplayTime, &test_interval);
    uids[2] = ScdAdd(test_scd , test_interval, DisplayTime, &test_interval);
    uids[3] = ScdAdd(test_scd , test_interval, DisplayTime, &test_interval);

    /* remove all tasks */
    for(; i < 4; ++i)
    {
        ScdRemove(test_scd, uids[i]);
    }

    if(1 != ScdIsEmpty(test_scd)) 
    {
        ScdDestroy(test_scd);
        printf("    %sLine %d - Scd should be empty\n",
               RED, __LINE__ - 4);
        return 1;
    }

    ScdDestroy(test_scd);
    return 0;
}

int TestAddTask()
{
    scd_t *test_scd = ScdCreate();
    size_t expected_size = 4;
    size_t actual_size = 0;
    size_t test_interval = 3;

    /* task adding tasks */
    ScdAdd(test_scd , test_interval, AddTasks, (void *)test_scd);

    /* adding stop task */
    ScdAdd(test_scd, 10, StopScd, test_scd);

    ScdRun(test_scd);

    actual_size = ScdSize(test_scd);

    if(actual_size != expected_size)
    {
        ScdDestroy(test_scd);
        printf("    %sLine %d - Expected Size: %ld, Actual Size: %ld\n",
               RED, __LINE__ - 4, expected_size, actual_size);
        return 1;
    }


    ScdDestroy(test_scd);
    return 0;
}

int TestRemove2()
{
    scd_t *test_scd = ScdCreate();
    size_t expected_size = 2;
    size_t actual_size = 0;
    size_t test_interval = 9;  

    /* add tasks */
    ScdAdd(test_scd , 1, SimpleTask, NULL);
    task_uid = ScdAdd(test_scd , 100, SimpleTask, NULL);


    /* adding task removing task */
    ScdAdd(test_scd , test_interval, RemoveTask, (void *)test_scd);

    /* adding stop task */
    ScdAdd(test_scd, 10, StopScd, test_scd);

    ScdRun(test_scd);

    actual_size = ScdSize(test_scd);

    if(actual_size != expected_size)
    {
        ScdDestroy(test_scd);
        printf("    %sLine %d - Expected Size: %ld, Actual Size: %ld\n",
               RED, __LINE__ - 4, expected_size, actual_size);
        return 1;
    }

    ScdDestroy(test_scd);
    return 0;
}

int TestFile()
{
    scd_t *test_scd = ScdCreate();
    size_t test_interval1 = 1;

    /* add print task */
    ScdAdd(test_scd , 1, DisplayTime, &test_interval1);

    /* add stop task */
    ScdAdd(test_scd, 1, StopFile, test_scd);

    /* add file creation task */
    ScdAdd(test_scd, 10, CreateStopFile, NULL); 

    ScdRun(test_scd);

    ScdDestroy(test_scd);    
    
    return 0;
}

int TestFlow()
{
    size_t test_interval1 = 1;
    size_t test_interval4 = 4;
    size_t test_interval5 = 5;
    size_t test_interval10 = 10;
    int expected_time = 25;
    time_t curr_time = 0;
    time_t start_time = 0;
    time_t total_time = 0;

    scd_t *test_scd = ScdCreate();

    /* sleep until time is round */
    curr_time = time(NULL);
    start_time = curr_time;
    sleep(10 - curr_time % 10);

    /* adding four tasks at different intervals */
    ScdAdd(test_scd , test_interval1, DisplayTime, &test_interval1);
    ScdAdd(test_scd , test_interval5, DisplayTime, &test_interval5);
    ScdAdd(test_scd , test_interval4, DisplayTime, &test_interval4);
    ScdAdd(test_scd , test_interval10, DisplayTime, &test_interval10); 

    /* adding stop task */
    ScdAdd(test_scd, 10, StopScd, test_scd);

    /* run scheduler and print starting + stop time*/
    curr_time = time(NULL);
    start_time = curr_time;
    printf("    Starting time:     %s", ctime(&curr_time));
    printf("---------------------------------------------------\n");
    ScdRun(test_scd);
    
    curr_time = time(NULL);
    printf("---------------------------------------------------\n");
    printf("    Ending time:       %s", ctime(&curr_time));
               
    /* sleep for 5 seconds */ 
    printf("\n    Sleeping for 5 seconds\n\n");
    sleep(5);

    curr_time = time(NULL);
    printf("    Starting time:     %s", ctime(&curr_time));
    printf("---------------------------------------------------\n");
  
    ScdRun(test_scd);

    curr_time = time(NULL);
    printf("---------------------------------------------------\n");
    printf("    Ending time:       %s", ctime(&curr_time));
    total_time = curr_time - start_time;

    if(expected_time != total_time)
    {
        ScdDestroy(test_scd);
        printf("    %sLine %d - Expected time: %d Actual time: %ld \n",
               RED, __LINE__ - 4, expected_time , total_time);
        return 1;
    }

    ScdDestroy(test_scd);

    return 0;
}

int main()
{
    RUN_TEST(TestCreateDestroy);
    RUN_TEST(TestIsEmptySize);
    RUN_TEST(TestRemove);
    RUN_TEST(TestRemove2);
    RUN_TEST(TestFile);
    RUN_TEST(TestFlow);
    RUN_TEST(TestAddTask);
    
    return 0;
}

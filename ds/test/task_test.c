#include <stdio.h> /* printf */
#include <time.h> /* time */
#include <unistd.h> /* sleep */

#include "../include/task.h" /* task queue header file */

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET_COLOR "\x1b[0m"
#define YELLOW "\x1b[93m"
#define BLUE "\x1b[36m"

#define RUN_TEST(test_name) \
    printf("\n%s[Calling: " #test_name "]\n\n%s", YELLOW, RESET_COLOR); \
    ++num_of_tests; \
    \
    if(0 == test_name()) \
    { \
        ++successful_tests; \
        printf("%s\n[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s\n[" #test_name " was completed unsuccessfully]\n", RED); \
    } \
    printf("%s\n", RESET_COLOR);

static int num_of_tests = 0;
static int successful_tests = 0;

void DisplayTimeToRun(task_t *task)
{
    time_t ttr = TaskGetTime(task);
    
    printf("Time to run:  %s", ctime(&ttr));
}

void DisplayTime()
{
   time_t curr_time = time(NULL);
    
    printf("Current Time: %s", ctime(&curr_time));

}

long SomeFunc(void *params)
{
    (void)params;

    printf("    Executing task: 23 23 23 23\n");
    return *((size_t *)params);
}

int TestCreateExecuteDestroyGet()
{
    time_t test_interval = 5;
    task_t *test_task = TaskCreate(test_interval, SomeFunc, &test_interval);
    time_t first_ttr = TaskGetTime(test_task);
    time_t second_ttr = 0;

    sleep(test_interval);

    TaskExecute(test_task);
    second_ttr = TaskGetTime(test_task);

    if(second_ttr != first_ttr + test_interval)
    {
         TaskDestroy(test_task);
         printf("    %sLine %d - Expected Run Time: %ld\n", 
                     RED, __LINE__ - 4, first_ttr + test_interval); 
         printf("    Line %d - Actual Run Time:   %ld\n", 
                                         __LINE__ - 6, second_ttr); 
        
        return 1;
    }

    TaskDestroy(test_task);

    return 0;
}

int main()
{
    RUN_TEST(TestCreateExecuteDestroyGet);
    
    return 0;
}

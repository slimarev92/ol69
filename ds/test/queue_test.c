#include <stdio.h> /* printf */

#include "../include/queue.h" /* header file */

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET_COLOR "\x1b[0m"
#define YELLOW "\x1b[93m"
#define BLUE "\x1b[36m"

#define RUN_TEST(test_name) \
    printf("%s[Calling: " #test_name "]\n%s", YELLOW, RESET_COLOR); \
    if(0 == test_name()) \
    { \
        printf("%s[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \
    } \
    printf("%s", RESET_COLOR);

static int num_of_tests = 0;
static int successful_tests = 0;

int TestEqDqPeek()
{
    queue_t *test_queue = QueueCreate();
    size_t i = 1;
    int enq_status = 0;
    size_t expected_size = 10;
    size_t actual_size = 0;
    size_t curr_data = 0;
    
    ++num_of_tests;

    for(; i <= 10; ++i)
    {
        enq_status = QueueEnqueue(test_queue, (void *)i);
        
        if(0 != enq_status)
        {
            QueueDestroy(test_queue);
            printf("    %sLine %d - Unsuccessfull Enque (i = %ld) \n",
               RED, __LINE__ - 4, i);

            return 1;
        }
    }

    actual_size = QueueSize(test_queue);

    if(expected_size != actual_size)
    {
        QueueDestroy(test_queue);
        printf("    %sLine %d - Expected Size: %ld \
               Actual Size: %ld (i = %ld) \n",
               RED, __LINE__ - 5, expected_size , actual_size ,i);

        return 1;
    }

    for(i = 1; i <= 10; ++i)
    {
        curr_data = (size_t)QueuePeek(test_queue);
        QueueDequeue(test_queue);

        if(curr_data != i)
        {
           QueueDestroy(test_queue);
           printf("    %sLine %d - Expected Value: %ld \
                         Actual Value: %ld (i = %ld) \n",
                     RED, __LINE__ - 5,  curr_data, i, i);

           return 1; 
        }
    }

    ++successful_tests;

    QueueDestroy(test_queue);
    return 0;
}

int TestAppend()
{
    queue_t *test_queue_src = QueueCreate();   
    queue_t *test_queue_dst = QueueCreate();
    size_t i = 1;
    size_t expected_size = 20;
    size_t actual_size = 0;
    size_t curr_data = 0;

    ++num_of_tests;
    
    for(; i <= 10; ++i)
    {
        QueueEnqueue(test_queue_dst, (void *)i);
    }

    for(i = 11; i <= 20; ++i)
    {
        QueueEnqueue(test_queue_src, (void *)i);
    }

    QueueAppend(test_queue_dst, test_queue_src);

    actual_size = QueueSize(test_queue_dst);

    if(expected_size != actual_size)
    {
        QueueDestroy(test_queue_dst);
        QueueDestroy(test_queue_src);
        printf("    %sLine %d - Expected Size: %ld \
               Actual Size: %ld (i = %ld) \n",
               RED, __LINE__ - 6, expected_size , actual_size ,i);

        return 1;
    }

      for(i = 1; i <= 20; ++i)
      {
        curr_data = (size_t)QueuePeek(test_queue_dst);
        QueueDequeue(test_queue_dst);

        if(curr_data != i)
        {
           QueueDestroy(test_queue_src);
           QueueDestroy(test_queue_dst);
           printf("    %sLine %d - Expected Value: %ld, Actual Value: %ld (i = %ld) \n",
                     RED, __LINE__ - 6, i,  curr_data, i);

           return 1; 
        }
    }


    ++successful_tests;

    QueueDestroy(test_queue_src);
    QueueDestroy(test_queue_dst);
    return 0;
}

int main()
{
    printf("\n");

    RUN_TEST(TestEqDqPeek);
    printf("\n");
    RUN_TEST(TestAppend);
    printf("\n");

    printf("%s", BLUE);
    printf("[Summary]\n    Successful tests: %d\n    Failed tests: %d\n\n", successful_tests, num_of_tests - successful_tests);
    printf("%s", RESET_COLOR);

    return 0;
}

#include <stdio.h> /* printf */

#include "pq.h" /* priority queue header file */

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET_COLOR "\x1b[0m"
#define YELLOW "\x1b[93m"
#define BLUE "\x1b[36m"

#define RUN_TEST(test_name) \
    printf("\n%s[Calling: " #test_name "]\n%s", YELLOW, RESET_COLOR); \
    ++num_of_tests; \
    \
    if(0 == test_name()) \
    { \
        ++successful_tests; \
        printf("%s[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \
    } \
    printf("%s\n", RESET_COLOR);

static int num_of_tests = 0;
static int successful_tests = 0;

struct person
{
    size_t age;
    size_t id;
};

int MultiplyBy(void *data, const void *arg)
{
    size_t *data_ptr = (size_t *)data;

    *data_ptr =  *data_ptr *  (*(size_t *)arg);

    return 0;
}  

int Compare(const void *data, const void *key, const void *params)
{
    (void)params;

    return (size_t)key == (size_t)data;
}

int IsLowerId(const void *data, const void *key, const void *params)
{
    (void)params;

    return ((struct person *)key)->id > ((struct person *)data)->id;
}

int IsBefore(const void *data, const void *key, const void *params)
{
    (void)params;

    return (size_t)key > (size_t)data;

    return 0;
}

int TestCreateEmptyDestroy()
{
    pq_t *test_pq = PQCreate(Compare);

    if(!PQIsEmpty(test_pq))
    {
            PQDestroy(test_pq);
            printf("    %sLine %d - Expected Empty PQ\n",
               RED, __LINE__ - 4);
            return 1;
    }

    PQDestroy(test_pq);

    return 0;
}

int TestEnqPeekDequeErase2()
{
    pq_t *test_pq = PQCreate(IsLowerId);
    struct person actual[] = {{16, 10}, {25, 9}, {23, 8}};
    struct person expected[] = {{16, 10}, {16, 10},
                                 {25, 9},  {25, 9},
                                 {23, 8},  {23, 8}};
         
    size_t i = 10;
    size_t expected_size = 6;
    size_t actual_size = 0;
    size_t curr_data = 0;

    for(i = 0; i < 3; ++i)
    {
        PQEnqueue(test_pq, (actual+i));
        PQEnqueue(test_pq, (actual+i));
    }

    actual_size = PQSize(test_pq);
    
    if(actual_size != expected_size)
    {
        PQDestroy(test_pq);
        printf("    %sLine %d - Expected Size: %ld, Actual Size: %ld\n",
               RED, __LINE__ - 4, expected_size, actual_size);
        return 1;
    }

    for(i = 0; i < 3; ++i)
    {
        curr_data =  ((struct person *)PQPeek(test_pq))->age;
        
        if(curr_data != expected[i].age)
        {
            PQDestroy(test_pq);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 4, expected[i].age, curr_data);
            return 1;
        }

        PQDequeue(test_pq);
    }

    PQDestroy(test_pq);

    return 0;
}

int TestEnqPeekDequeErase()
{
    pq_t *test_pq = PQCreate(IsBefore);
    size_t i = 10;
    size_t expected_size = 10;
    size_t actual_size = 0;
    size_t curr_data = 0;

    for(; i >= 1; --i)
    {
        PQEnqueue(test_pq, (void *)i);
    }

    actual_size = PQSize(test_pq);
    
    if(actual_size != expected_size)
    {
        PQDestroy(test_pq);
        printf("    %sLine %d - Expected Size: %ld, Actual Size: %ld\n",
               RED, __LINE__ - 4, expected_size, actual_size);
        return 1;
    }

    for(i = 10; i >= 1; --i)
    {
        curr_data =  (size_t)PQPeek(test_pq);
        
        if(curr_data != i)
        {
            PQDestroy(test_pq);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 4, i, curr_data);
            return 1;
        }

        PQDequeue(test_pq);
    }

    PQDestroy(test_pq);

    return 0;
}

int TestErase()
{
    pq_t *test_pq = PQCreate(IsBefore);
    size_t i = 10;
    size_t expected_size = 10;
    size_t actual_size = 0;
    size_t curr_data = 0;

    for(; i >= 1; --i)
    {
        PQEnqueue(test_pq, (void *)i);
    }

    actual_size = PQSize(test_pq);
    
    if(actual_size != expected_size)
    {
        PQDestroy(test_pq);
        printf("    %sLine %d - Expected Size: %ld, Actual Size: %ld\n",
               RED, __LINE__ - 4, expected_size, actual_size);
        return 1;
    }

    for(i = 10; i >= 1; --i)
    {
        curr_data =  (size_t)PQPeek(test_pq);
        
        if(curr_data != i)
        {
            PQDestroy(test_pq);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 4, i, curr_data);
            return 1;
        }

        PQDequeue(test_pq);
    }

    PQDestroy(test_pq);

    return 0;


}

int TestClear()
{
    pq_t *test_pq = PQCreate(IsBefore);
    size_t i = 10;

    for(; i >= 1; --i)
    {
        PQEnqueue(test_pq, (void *)i);
    }

    PQClear(test_pq);

    if(!PQIsEmpty(test_pq))
    {
            PQDestroy(test_pq);
            printf("    %sLine %d - Expected Empty PQ\n",
               RED, __LINE__ - 4);
            return 1;
    }


    PQDestroy(test_pq);

    return 0;
}

int main()
{
    RUN_TEST(TestCreateEmptyDestroy);
    RUN_TEST(TestEnqPeekDequeErase);
    RUN_TEST(TestEnqPeekDequeErase2);
    RUN_TEST(TestClear);

    return 0;
}

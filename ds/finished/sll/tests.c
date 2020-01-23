#include <stdio.h> /* printf */

#include "sll.h" /* header file */

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

void PrintNums(sll_t *sll)
{
    it_t iter = SLLBegin(sll);
    it_t end = SLLEnd(sll);
    size_t curr_data = 0;
    size_t i = 0;

    while(iter != end)
    {
        ++i;
        curr_data = (size_t)SLLGetData(iter);
        printf("Datum %3ld: %3ld\n", i, curr_data);
        iter = SLLNext(iter);
    }
}

int TestFlip()
{    
    sll_t *test_sll = SLLCreate();  
    size_t i = 0;
    size_t curr_data = 0;
    it_t iter = NULL;

    ++num_of_tests;

    for(; i < 10; ++i)
    {
        SLLPushBack(test_sll, (void *)i);
    }

    SLLFLip(test_sll);

    iter = SLLBegin(test_sll);

    for(i = 0; i < 10; ++i)
    {
        curr_data = (size_t)SLLGetData(iter);
        if(curr_data != (9 -i))
        {
            SLLDestroy(test_sll);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 4, i, curr_data);
            return 1;
        }     
        
        iter = SLLNext(iter);
    }

    ++successful_tests;

    SLLDestroy(test_sll);
    return 0;
}

int TestPushFrontSize()
{
    sll_t *test_sll = SLLCreate();  
    int expected_size = 10;
    int actual_size = 0;
    size_t i = 0;
    size_t curr_data = 0;
    it_t iter = NULL;

    ++num_of_tests;

    for(; i < 10; ++i)
    {
        SLLPushFront(test_sll, (void *)(9 - i));
    }

    actual_size = SLLSize(test_sll);

    if(expected_size != actual_size)
    {
         SLLDestroy(test_sll);
         printf("   %sLine %d - Expected Size: %d, Actual Size: %d\n",
               RED, __LINE__ - 4,expected_size, actual_size);
        return 1;
    }

    iter = SLLBegin(test_sll);

    for(i = 0; i < 10; ++i)
    {
        curr_data = (size_t)SLLGetData(iter);
        if(curr_data != i)
        {
            SLLDestroy(test_sll);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 4, i, curr_data);
            return 1;
        }     
        
        iter = SLLNext(iter);
    }

    ++successful_tests;

    SLLDestroy(test_sll);
    return 0;
}

int TestPushBack()
{
    sll_t *test_sll = SLLCreate();  
    size_t i = 0;
    size_t curr_data = 0;
    it_t iter = NULL;

    ++num_of_tests;

    for(; i < 10; ++i)
    {
        SLLPushBack(test_sll, (void *)i);
    }

    iter = SLLBegin(test_sll);

    for(i = 0; i < 10; ++i)
    {
        curr_data = (size_t)SLLGetData(iter);

        if(curr_data != i)
        {
            SLLDestroy(test_sll);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 4, i, curr_data);
            return 1;
        }     

        iter = SLLNext(iter);
    }

    ++successful_tests;

    SLLDestroy(test_sll);
    return 0;
}

int NumCompare(const void *data, const void *key)
{
    return (size_t)data == (size_t)key;
}

int TestFind()
{    
    sll_t *test_sll = SLLCreate();  
    size_t i = 0;
    it_t iter = NULL;
    it_t start = NULL;
    it_t end = SLLEnd(test_sll);

    ++num_of_tests;

    for(; i < 10; ++i)
    {
        SLLPushBack(test_sll, (void *)i);
    }

    start = SLLBegin(test_sll);

    for(i = 0; i < 10; ++i)
    {
        iter = SLLFind(start, end, (void *)i, NumCompare);

        if(iter == end)
        {
             SLLDestroy(test_sll);
             printf("   %sLine %d - Couldn't find node containing %ld\n",
               RED, __LINE__ - 4, i);
            return 1;
        }      
    }

    ++successful_tests;

    SLLDestroy(test_sll);
    return 0;
}

int TestErase()
{
    sll_t *test_sll = SLLCreate();  
    size_t i = 0;
    size_t curr_data = 0;
    it_t iter = NULL;
    it_t start = NULL;
    it_t end = SLLEnd(test_sll);

    ++num_of_tests;

    for(; i < 10; ++i)
    {
        SLLPushBack(test_sll, (void *)i);
    }

    start = SLLBegin(test_sll);
    iter = start;

    SLLErase(test_sll, SLLFind(start, end, (void *)9, NumCompare));
    SLLErase(test_sll, SLLFind(start, end, (void *)0, NumCompare));

    iter = SLLBegin(test_sll);

    for(i = 1; i < 8; ++i)
    {
        curr_data = (size_t)SLLGetData(iter);
  
        if(curr_data != i)
        {
            SLLDestroy(test_sll);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 4, i, curr_data);
            return 1;
        }

        iter = SLLNext(iter);
    }

    ++successful_tests;
    
    SLLDestroy(test_sll);
    return 0;
}

int TestPopFront()
{
    sll_t *test_sll = SLLCreate();  
    size_t i = 0;
    size_t curr_data = 0;
    it_t iter = NULL;
    it_t start = NULL;

    ++num_of_tests;

    for(; i < 10; ++i)
    {
        SLLPushBack(test_sll, (void *)i);
    }

    start = SLLBegin(test_sll);
    iter = start;

    SLLPopFront(test_sll);
    SLLPopFront(test_sll);
    SLLPopFront(test_sll);

    iter = SLLBegin(test_sll);

    for(i = 3; i < 8; ++i)
    {
        curr_data = (size_t)SLLGetData(iter);
  
        if(curr_data != i)
        {
            SLLDestroy(test_sll);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 4, i, curr_data);
            return 1;
        }

        iter = SLLNext(iter);
    }

    ++successful_tests;
    
    SLLDestroy(test_sll);
    return 0;
}

int TestPopBack()
{
    sll_t *test_sll = SLLCreate();  
    size_t i = 0;
    size_t curr_data = 0;
    it_t iter = NULL;
    it_t start = NULL;

    ++num_of_tests;

    for(; i < 10; ++i)
    {
        SLLPushBack(test_sll, (void *)i);
    }

    start = SLLBegin(test_sll);
    iter = start;

    SLLPopBack(test_sll);
    SLLPopBack(test_sll);
    SLLPopBack(test_sll);

    iter = SLLBegin(test_sll);

    for(i = 0; i < 7; ++i)
    {
        curr_data = (size_t)SLLGetData(iter);
  
        if(curr_data != i)
        {
            SLLDestroy(test_sll);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 4, i, curr_data);
            return 1;
        }

        iter = SLLNext(iter);
    }

    ++successful_tests;
    
    SLLDestroy(test_sll);
    return 0;
}

int MultiplyBy(void *data, const void *arg)
{
     int *data_ptr = (int *)data;

    *data_ptr = *data_ptr * (*(int *)arg);

    (void)arg;
    return 0;
}

int TestForEach()
{
    sll_t *test_sll = SLLCreate();  
    int i = 0;
    int by = 3;
    int *curr_data = 0;
    it_t iter = NULL;
    static int nums[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    ++num_of_tests;

    for(; i < 10; ++i)
    {
        SLLPushBack(test_sll, (void *)(nums + i));
    }

    iter = SLLBegin(test_sll);
   
    SLLForEach(SLLBegin(test_sll), SLLEnd(test_sll), &by, MultiplyBy);

    for(i = 0; i < 10; ++i)
    {
        curr_data = (int *)SLLGetData(iter);
  
        if(*curr_data != (i * by))
        {
            SLLDestroy(test_sll);
            printf("    %sLine %d - Expected Data: %d, Actual Data: %d\n",
               RED, __LINE__ - 4, i * by, *curr_data);
            return 1;
        }

        iter = SLLNext(iter);
    }

    ++successful_tests;
    
    SLLDestroy(test_sll);
    return 0;
}

int main()
{
    printf("\n");

    RUN_TEST(TestPushFrontSize);
    RUN_TEST(TestPushBack);
    RUN_TEST(TestFind);
    RUN_TEST(TestErase);
    RUN_TEST(TestPopFront);
    RUN_TEST(TestPopBack); 
    RUN_TEST(TestForEach);
    RUN_TEST(TestFlip);

    printf("\n%s", BLUE);
    printf("[Summary]\n    Successful tests: %d\n    Failed tests: %d\n\n", successful_tests, num_of_tests - successful_tests);
    printf("%s", RESET_COLOR);

    return 0;
}

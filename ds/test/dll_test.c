#include <stdio.h> /* printf */

#include "../include/dll.h" /* DLL header file */

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

int Compare(const void *data, const void *key, const void* params)
{
    (void)params;

    return (size_t)data == (size_t)key;
} 

int MultiplyBy(void *data, const void *arg)
{
    size_t *data_ptr = (size_t *)data;

    *data_ptr =  *data_ptr *  (*(size_t *)arg);

    return 0;
}  

int TestCreatePushSize()
{
    dll_t *test_dll = DLLCreate();  
    it_t iter = NULL;
    size_t expected_size = 10;
    size_t actual_size = 0;
    size_t curr_data = 0;
    size_t i = 1;

    ++num_of_tests;

    if(1 != DLLIsEmpty(test_dll))
    {
        DLLDestroy(test_dll);
        printf("    %sLine %d - IsEmpty returned 0\n", RED, __LINE__ - 3);

        return 1;
    }

    for(; i <= 10; ++i)
    {
        DLLPushBack(test_dll, (void *)i);
    }
    
    actual_size = DLLSize(test_dll);

    if(actual_size != expected_size)
    {
            DLLDestroy(test_dll);
            printf("    %sLine %d - Expected Size: %ld, Actual Size: %ld\n",
               RED, __LINE__ - 4, expected_size, actual_size);
            return 1;
    }

    iter = DLLBegin(test_dll);

    for(i = 1; i <= 10; ++i)
    {
        curr_data = (size_t)DLLGetData(iter);

        if(curr_data != i)
        {
            DLLDestroy(test_dll);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 4, i, curr_data);
            return 1;
        }

        iter = DLLNext(iter);
    }

    ++successful_tests;
    
    DLLDestroy(test_dll);
    return 0;
}

int TestErase()
{
    dll_t *test_dll = DLLCreate();  
    it_t iter = NULL;
    size_t expected_size = 0;
    size_t actual_size = 0;
    size_t i = 1;

    ++num_of_tests;

    for(; i <= 10; ++i)
    {
        DLLPushFront(test_dll, (void *)i);
    }
    
    iter = DLLBegin(test_dll);

    for(i = 1; i <= 10; ++i)
    {
        DLLErase(iter);

        iter = DLLBegin(test_dll);
    }

    actual_size = DLLSize(test_dll);

    if(actual_size != expected_size)
    {
            DLLDestroy(test_dll);
            printf("    %sLine %d - Expected Size: %ld, Actual Size: %ld\n",
               RED, __LINE__ - 4, expected_size, actual_size);
            return 1;
    }
    
    ++successful_tests;
    
    DLLDestroy(test_dll);
    return 0;
}

int TestFind()
{
    dll_t *test_dll = DLLCreate();  
    it_t iter = NULL;
    it_t end = DLLEnd(test_dll);
    it_t curr_find = NULL;
    size_t i = 1;

    ++num_of_tests;
 
    for(; i <= 10; ++i)
    {
        DLLPushBack(test_dll, (void *)i);
    }
    
    iter = DLLBegin(test_dll);
    
    for(i = 1; i <= 10; ++i)
    {
        curr_find = DLLFind(iter, end, Compare,  (void *)i  ,NULL);

        if(curr_find == end)
        {
            DLLDestroy(test_dll);
            printf("    %sLine %d - Couldn't find element %ld\n",
               RED, __LINE__ - 4, i);
            return 1;
        }

        iter = DLLNext(iter);
    }

    ++successful_tests;
    
    DLLDestroy(test_dll);
    return 0;
}

int TestForEach()
{
    dll_t *test_dll = DLLCreate();  
    size_t i = 1;
    size_t by = 10;
    size_t *curr_data = 0;
    it_t iter = NULL;
    static size_t nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    ++num_of_tests;

    for(; i <= 10; ++i)
    {
        DLLPushBack(test_dll, ((size_t *)nums + i - 1));
    }

    iter = DLLBegin(test_dll);

    DLLForEach(DLLBegin(test_dll), DLLEnd(test_dll),  MultiplyBy , &by);

    for(i = 1; i <= 10; ++i)
    {
        curr_data = (size_t *)DLLGetData(iter);

        if(*curr_data != (i * by))
        {
            DLLDestroy(test_dll);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 4, i * by, *curr_data);
            return 1;
        }

        iter = DLLNext(iter);
    }

    ++successful_tests;
    
    DLLDestroy(test_dll);
    return 0;
}

int TestSplice()
{
    dll_t *test_to = DLLCreate();  
    dll_t *test_from = DLLCreate();  
    it_t from = NULL;
    it_t to = NULL;
    it_t iter = NULL;
    size_t i = 1;
    size_t curr_data = 0;
    it_t where = NULL;

    ++num_of_tests;

    for(; i <= 2; ++i)
    {
        DLLPushBack(test_to, (void *)i);
    }

    for(i = 3; i <= 5; ++i)
    {
        DLLPushBack(test_from, (void *)i);
    }

    for(i = 6; i <= 10; ++i)
    {
        DLLPushBack(test_to, (void *)i);
    }

    from = DLLBegin(test_to);
    to = DLLEnd(test_to);

    where = DLLFind(from, to, Compare, (void *)6, NULL);

    from = DLLBegin(test_from);
    to = DLLEnd(test_from);

    DLLSpliceBefore(where, from, to);
   
    iter = DLLBegin(test_to);

    for(i = 1; i <= 10; ++i) /* test forwards */
    {
        curr_data = (size_t)DLLGetData(iter);

        if(curr_data != i)
        {
            DLLDestroy(test_from);
            DLLDestroy(test_to);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 5, i, curr_data);
            return 1;
        }

        iter = DLLNext(iter);
    }

   iter = DLLPrev(DLLEnd(test_to));

    for(i = 10; i >= 1; --i)  /* test backwards */
    {
        curr_data = (size_t)DLLGetData(iter);

        if(curr_data != i)
        {
            DLLDestroy(test_from);
            DLLDestroy(test_to);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 5, i, curr_data);
            return 1;
        }

        iter = DLLPrev(iter);
    }

    ++successful_tests;
    
    DLLDestroy(test_from);
    DLLDestroy(test_to);
    return 0;
}

int main()
{
    printf("\n");
    RUN_TEST(TestCreatePushSize);
    printf("\n");
    RUN_TEST(TestErase);
    printf("\n");
    RUN_TEST(TestFind);
    printf("\n");
    RUN_TEST(TestForEach);
    printf("\n");
    RUN_TEST(TestSplice);
    printf("\n");
   

    return 0;
}

#include <stdio.h> /* printf */

#include "srtl.h" /* DLL header file */

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

int IsBefore(const void *data, const void *key, const void *params)
{
    (void)params;

    return (size_t)key <= (size_t)data;

    return 0;
}

void DisplayNums(srtl_t *srtl)
{
    it_t iter = SrtLBegin(srtl);
    it_t end = SrtLEnd(srtl);

    for(; iter != end; iter = SrtLNext(iter))
    {
        printf("%ld\n", (size_t)SrtLGetData(iter));
    }
}

int TestMerge()
{
    srtl_t *test_src = SrtLCreate(IsBefore, NULL); 
    srtl_t  *test_dst = SrtLCreate(IsBefore, NULL); 
    size_t i = 0;
    size_t curr_data = 0;
    it_t iter = NULL;


    SrtLInsert(test_dst, (void *)1);
    SrtLInsert(test_dst, (void *)2);
    SrtLInsert(test_dst, (void *)5);
    SrtLInsert(test_dst, (void *)6);
    SrtLInsert(test_dst, (void *)9);
  
    SrtLInsert(test_src, (void *)3);
    SrtLInsert(test_src, (void *)4);
    SrtLInsert(test_src, (void *)7);
    SrtLInsert(test_src, (void *)8);
    SrtLInsert(test_src, (void *)10);

    SrtLMerge(test_dst, test_src);

    iter = SrtLBegin(test_dst);

    for(i = 1; i <= 10; ++i)
    {
        curr_data = (size_t)SrtLGetData(iter);
        
        if(curr_data != i)
        {
            SrtLDestroy(test_src);
            SrtLDestroy(test_dst);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 5, i, curr_data);
            return 1;
        }

        iter = SrtLNext(iter);
    }

    iter = SrtLPrev(SrtLEnd(test_dst));

    for(i = 10; i >= 1; --i)
    {
        curr_data = (size_t)SrtLGetData(iter);
        
        if(curr_data != i)
        {
            SrtLDestroy(test_src);
            SrtLDestroy(test_dst);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 5, i, curr_data);
            return 1;
        }

        iter = SrtLPrev(iter);
    }
  
    SrtLDestroy(test_src);
    SrtLDestroy(test_dst);  
    return 0;
}

int TestCreateDestroyEmpty()
{
    srtl_t *test_srtl = SrtLCreate(IsBefore, NULL); 

    if(1 !=  SrtLIsEmpty(test_srtl))
    {
        SrtLDestroy(test_srtl);
        printf("    %sLine %d - Expected Size: 0, Actual Size: %ld\n",
               RED, __LINE__ - 4, SrtLSize(test_srtl));
        return 1;
    }

    SrtLDestroy(test_srtl);
    return 0;
}


void DisplayNumsPtr(srtl_t *srtl)
{
    it_t iter = SrtLBegin(srtl);
    it_t end = SrtLEnd(srtl);

    for(; iter != end; iter = SrtLNext(iter))
    {
        printf("%ld\n", *(size_t *)SrtLGetData(iter));
    }
}

int TestInsertSizePop()
{
    srtl_t *test_srtl = SrtLCreate(IsBefore, NULL); 
    size_t i = 0;
    size_t expected_size = 20;
    size_t actual_size = 0;
    size_t curr_data = 0;
    it_t iter = NULL;

    for(i = 20; i >= 11; --i)
    {
         SrtLInsert(test_srtl, (void *)i);
    }

    for(i = 10; i >= 1; --i)
    {
         SrtLInsert(test_srtl, (void *)i);
    }

    iter = SrtLBegin(test_srtl);

    for(i = 1; i <= 20; ++i)
    {
        curr_data = (size_t)SrtLGetData(iter);

        if(curr_data != i)
        {
            SrtLDestroy(test_srtl);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 4, i, curr_data);
            return 1;
        }

        iter = SrtLNext(iter);
    }

    actual_size = SrtLSize(test_srtl);

    if(actual_size != expected_size)
    {
        SrtLDestroy(test_srtl);
        printf("    %sLine %d - Expected Size: %ld, Actual Size: %ld\n",
               RED, __LINE__ - 4, expected_size, actual_size);
        return 1;
    }

    for(i = 0; i < 20; ++i)
    {
        SrtLPopBack(test_srtl);
    }

    if(1 != SrtLIsEmpty(test_srtl))
    {
        SrtLDestroy(test_srtl);
        printf("    %sLine %d - Expected Size: 0, Actual Size: %ld\n",
               RED, __LINE__ - 4, SrtLSize(test_srtl));
        return 1;
    }

    SrtLDestroy(test_srtl);
    return 0;
}

int TestFind()
{
    srtl_t *test_srtl = SrtLCreate(IsBefore, NULL); 
    size_t i = 1;
    size_t curr_data = 0;
    it_t iter = NULL;
    it_t end = SrtLEnd(test_srtl);

    for(; i <= 10; ++i)
    {
         SrtLInsert(test_srtl, (void *)i);
    }

    iter = SrtLBegin(test_srtl);

    for(i = 1; i <= 10; ++i)
    {
        curr_data = (size_t)SrtLGetData(SrtLFind(iter, end , Compare,
                    (void *)i, NULL));

        if(curr_data != i)
        {
            SrtLDestroy(test_srtl);
            printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld\n",
               RED, __LINE__ - 4, i, curr_data);
            return 1;
        }

        iter = SrtLNext(iter);
    }

    SrtLDestroy(test_srtl);
    return 0;
}

int TestForEach()
{
    srtl_t *test_srtl = SrtLCreate(IsBefore, NULL);  
    size_t i = 1;
    size_t by = 23;
    size_t *curr_data = 0;
    it_t iter = NULL;
    static size_t nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for(i = 0; i < 10; ++i)
    {
         SrtLInsert(test_srtl, (void *)&nums[i]);
    }

    iter = SrtLBegin(test_srtl);

    SrtLForEach(SrtLBegin(test_srtl), SrtLEnd(test_srtl),  MultiplyBy , &by);

    for(i = 1; i <= 10; ++i)
    {
        curr_data = (size_t *)SrtLGetData(iter);

        if(*curr_data != (i * by))
        {
            SrtLDestroy(test_srtl);
          printf("    %sLine %d - Expected Data: %ld, Actual Data: %ld i=%ld\n",
               RED, __LINE__ - 4, i * by, *curr_data, i);
            return 1;
        }

        iter = SrtLNext(iter);
    }

    ++successful_tests;
    
    SrtLDestroy(test_srtl);
    return 0;
}

int TestErase()
{
    srtl_t *test_srtl = SrtLCreate(IsBefore, NULL); 
    size_t i = 1;
    it_t curr = NULL;
    it_t iter = NULL;
    it_t end = SrtLEnd(test_srtl);

    for(; i <= 10; ++i)
    {
         SrtLInsert(test_srtl, (void *)i);
    }

    iter = SrtLBegin(test_srtl);

    for(i = 1; i <= 10; ++i)
    {
        curr = SrtLFind(iter, end , Compare, (void *)i, NULL);

        SrtLErase(curr);

        iter = SrtLNext(iter);
    }

    if(!SrtLIsEmpty(test_srtl))
    {
        SrtLDestroy(test_srtl);
        printf("    %sLine %d - Expected Size: 0, Actual Size: %ld\n",
               RED, __LINE__ - 4, SrtLSize(test_srtl));
        return 1;
    }

    SrtLDestroy(test_srtl);
    return 0;
}

int main()
{
    RUN_TEST(TestCreateDestroyEmpty);
    RUN_TEST(TestInsertSizePop);
    RUN_TEST(TestFind);
    RUN_TEST(TestErase);
    RUN_TEST(TestForEach);
    RUN_TEST(TestMerge);

    return 0;
}

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "../include/vsma.h" /*  VSMA header file */

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

int TestFreeCountSimple()
{
    size_t pool_size = 100;
    void *pool = malloc(pool_size);
    vsma_t *test_vsma = NULL;
    #ifndef NDEBUG  
    size_t expected_count = 40;
    #else
    size_t expected_count = 64; 
    #endif   

    size_t actual_count = 0;

    void *alloc1 = NULL;
    void *alloc2 = NULL;
    void *alloc3 = NULL;

    test_vsma = VSMAInit(pool, pool_size);
   
    alloc1 = VSMAAlloc(test_vsma, 8); 
    alloc2 = VSMAAlloc(test_vsma, 8); 
    alloc3 = VSMAAlloc(test_vsma, 16); 

    *(size_t *)alloc1 = 23;
    *(size_t *)alloc2 = 23;
    *(size_t *)alloc3 = 23;

    VSMAFree(alloc2);
    VSMAFree(alloc3);

    actual_count = VSMACount(test_vsma);

    if(expected_count != actual_count)
    {
        free(pool);
        printf("    %sLine %d - Expected count: %ld, Actual count: %ld\n",
               RED, __LINE__ - 4, expected_count, actual_count);
        return 1;
    }

    free(pool);
    return 0;
}

int TestFreeCountAdvanced()
{
    size_t pool_size = 100;
    void *pool = malloc(pool_size);
    vsma_t *test_vsma = NULL;
    #ifndef NDEBUG  
    size_t expected_count = 16;
    #else
    size_t expected_count = 40; 
    #endif   
    size_t actual_count = 0;
    void *alloc1 = NULL;
    void *alloc2 = NULL;
    void *alloc3 = NULL;
   
    test_vsma = VSMAInit(pool, pool_size);
    alloc1 = VSMAAlloc(test_vsma, 7); 
    alloc2 = VSMAAlloc(test_vsma, 8); 
    alloc3 = VSMAAlloc(test_vsma, 10); 

    VSMAFree(alloc1);
    VSMAFree(alloc2);
    VSMAFree(alloc3);

    alloc1 = VSMAAlloc(test_vsma, 30); 

    actual_count = VSMACount(test_vsma);

    if(expected_count != actual_count)
    {
        free(pool);
        printf("    %sLine %d - Expected count: %ld, Actual count: %ld\n",
               RED, __LINE__ - 4, expected_count, actual_count);
        return 1;
    }

    free(pool);
    return 0;
}

int TestFreeCountLargest()
{
    size_t pool_size = 100;
    void *pool = malloc(pool_size);
    vsma_t *test_vsma = NULL;
    #ifndef NDEBUG  
    size_t expected_count = 16;
    size_t expected_largest = 16;
    #else
    size_t expected_count = 16; 
    size_t expected_largest = 16;
    #endif   

    size_t actual_count = 0;
    size_t actual_largest = 0;

    void *alloc1 = NULL;
    void *alloc2 = NULL;
    void *alloc3 = NULL;
   
    test_vsma = VSMAInit(pool, pool_size);
    alloc1 = VSMAAlloc(test_vsma, 7); 
    alloc2 = VSMAAlloc(test_vsma, 8); 
    alloc3 = VSMAAlloc(test_vsma, 22);

    *(size_t *)alloc1 = 23;
    *(size_t *)alloc2 = 23;
    
    if(NULL != alloc3)
    {
        *(size_t *)alloc3 = 23;
    }


    actual_count = VSMACount(test_vsma);
    actual_largest = VSMAFindLargestFree(test_vsma);

    if(expected_count != actual_count)
    {
        free(pool);
        printf("    %sLine %d - Expected count: %ld, Actual count: %ld\n",
               RED, __LINE__ - 4, expected_count, actual_count);
        return 1;
    }

     if(expected_largest != actual_largest)
    {
        free(pool);
        printf("    %sLine %d - Expected largest: %ld, Actual largest: %ld\n",
               RED, __LINE__ - 4, expected_largest, actual_largest);
        return 1;
    }

    free(pool);
    return 0;
}

int TestAllocSimple()
{
    size_t pool_size = 100;
    void *pool = malloc(pool_size);
    vsma_t *test_vsma = NULL;
    void *simple_alloc = NULL;
    #ifndef NDEBUG  
    size_t expected_diff = 64;
    #else
    size_t expected_diff = 40;
    #endif   
    size_t actual_diff = 0;
   
    test_vsma = VSMAInit(pool, pool_size);

    simple_alloc = VSMAAlloc(test_vsma, 7); 
    simple_alloc = VSMAAlloc(test_vsma, 7); 
    simple_alloc = VSMAAlloc(test_vsma, 10); 
    actual_diff = (size_t)simple_alloc - (size_t)test_vsma;

    if(expected_diff != actual_diff)
    {
        free(pool);
        printf("    %sLine %d - Expected Offset: %ld, Actual Offset: %ld\n",
               RED, __LINE__ - 4, expected_diff, actual_diff);
        return 1;
    }

    free(pool);
    return 0;
}

int main()
{
    RUN_TEST(TestAllocSimple);
    RUN_TEST(TestFreeCountSimple);
    RUN_TEST(TestFreeCountAdvanced);
    RUN_TEST(TestFreeCountLargest);

    return 0;
}

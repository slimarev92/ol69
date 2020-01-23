#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "../include/fsma.h" /*  scheduler header file */

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

int TestInitCount()
{
    size_t block_size = 5;
    size_t n_blocks = 5;
    size_t suggested_size = FSMASuggestSize(n_blocks, block_size);
    void *pool = malloc(suggested_size);
    fsma_t *test_fsma = FSMAInit(pool, suggested_size, block_size);
    size_t expected_blocks = 5;
    size_t actual_blocks =  FSMACountFree(test_fsma);

    if(expected_blocks != actual_blocks)
    {
        free(pool);
        printf("    %sLine %d - Expected Blocks: %ld, Actual Blocks: %ld\n",
               RED, __LINE__ - 4, expected_blocks, actual_blocks);
        return 1;
    }
    
    free(pool);
    return 0;
}

int TestAlloc()
{
    size_t block_size = 5;
    size_t n_blocks = 10;
    size_t suggested_size = FSMASuggestSize(n_blocks, block_size);
    void *pool = malloc(suggested_size);
    fsma_t *test_fsma = FSMAInit(pool, suggested_size, block_size);
    size_t allocated_blocks = 4;
    size_t expected_blocks = n_blocks - allocated_blocks;
    size_t actual_blocks =  0;
    size_t i = 0;

    for(; i < allocated_blocks; ++i)
    {
        FSMAAlloc(test_fsma);
    }

    actual_blocks =  FSMACountFree(test_fsma);

    if(expected_blocks != actual_blocks)
    {
        free(pool);
        printf("    %sLine %d - Expected Blocks: %ld, Actual Blocks: %ld\n",
               RED, __LINE__ - 4, expected_blocks, actual_blocks);
        return 1;
    }
 
    free(pool);
    return 0;
}

int TestFree()
{
    size_t block_size = 5;
    size_t n_blocks = 10;
    size_t suggested_size = FSMASuggestSize(n_blocks, block_size);
    void *pool = malloc(suggested_size);
    fsma_t *test_fsma = FSMAInit(pool, suggested_size, block_size);
    size_t allocated_blocks = 4;
    size_t blocks_to_free = 2;
    size_t expected_blocks = n_blocks - (allocated_blocks * 2) + blocks_to_free;
    size_t actual_blocks =  0;
    size_t i = 0;
    static void *blocks[10];

    for(; i < allocated_blocks; ++i)
    {
        blocks[i] = FSMAAlloc(test_fsma);
        *((size_t *)blocks[i]) = i;
    }

    for(i = 0; i < blocks_to_free; ++i)
    {
        FSMAFree(blocks[i]);
    }

    for(i = 0; i < allocated_blocks; ++i)
    {
        blocks[i] = FSMAAlloc(test_fsma);
        *((size_t *)blocks[i]) = i;
    }

    actual_blocks =  FSMACountFree(test_fsma);

    if(expected_blocks != actual_blocks)
    {
        free(pool);
        printf("    %sLine %d - Expected Blocks: %ld, Actual Blocks: %ld\n",
               RED, __LINE__ - 4, expected_blocks, actual_blocks);
        return 1;
    }

    free(pool);
    return 0;
}

int main()
{
    RUN_TEST(TestInitCount);
    RUN_TEST(TestAlloc);
    RUN_TEST(TestFree);
    
    return 0;
}

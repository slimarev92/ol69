#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <time.h> /* time */

#include "avl.h" /* bst header file */

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[93m"
#define RESET_COLOR "\x1b[0m"

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
static int g_i = 0;
static int g_verbose = 0;
static size_t g_nums[7]; 

static int Compare(const void *d1, const void *d2, const void *params)
{
    (void)params;

    return (size_t)d1 - (size_t)d2;
}

static int PrintData(void *data, const void *args)
{
    int verbose = *(int *)args;

    (void)data;

    /* for diagnostic purposes */
    if(verbose)
    {
        printf("\tcurr data %ld\n", (size_t)data); 
    }

    return 0;
}

static int FillNums(void *data, const void *args)
{
    size_t *nums = (size_t *)args;

    nums[g_i] = (size_t)data;
    ++g_i;

    return 0;
}

static int GetData(void *data, const void *args)
{
    (void)args;

    g_nums[g_i] = (size_t)data;
    ++g_i;
    
    /* for diagnostic purposes */
    /* printf("\tcurr data %ld\n", (size_t)data); */
   
    return 0;
}

static int TestFind()
{
    avl_t *test_avl = AVLCreate(Compare, NULL);
    size_t nums[] = {2, 3, 4, 5, 6, 7, 9};
    size_t i = 0;
    size_t curr_find = 0;

    if(1 != AVLIsEmpty(test_avl))
    {
        AVLDestroy(test_avl);
        printf("\t%sLine %d - Expected empty tree\n", RED, __LINE__ - 3);
        return 1;
    }
        
    /* fill tree */
    AVLInsert(test_avl, (void *)(size_t)5);
    AVLInsert(test_avl, (void *)(size_t)3);
    AVLInsert(test_avl, (void *)(size_t)7);
    AVLInsert(test_avl, (void *)(size_t)2);
    AVLInsert(test_avl, (void *)(size_t)4);
    AVLInsert(test_avl, (void *)(size_t)6);
    AVLInsert(test_avl, (void *)(size_t)9); 

    for(; 8 > i + 1; ++i)
    {  
        curr_find = (size_t)AVLFind(test_avl, (void *)nums[i]);

        if(curr_find != nums[i])
        {
            AVLDestroy(test_avl);
            printf("\t%sLine %d -couldn't find %ld\n",
                                                    RED, __LINE__ - 4, nums[i]);
            return 1;
        }
    }
 
    if(NULL != AVLFind(test_avl, (void *)(size_t)23))
    {
            AVLDestroy(test_avl);
            printf("\t%sLine %d - Find for data not in tree returned !NULL\n",
                                                             RED, __LINE__ - 4);
            return 1;
    }
    
    AVLDestroy(test_avl);
    return 0;
}

static int TestBalance1() /* finish this !!!!!!! */
{
    avl_t *test_avl = AVLCreate(Compare, NULL);
    size_t expected_height = 1;
    size_t actual_height = 0;

    if(1 != AVLIsEmpty(test_avl))
    {
        AVLDestroy(test_avl);
        printf("\t%sLine %d - Expected empty tree\n", RED, __LINE__ - 3);
        return 1;
    }
        
    /* fill tree  - simple balancing (insert) */
    AVLInsert(test_avl, (void *)(size_t)4);
    AVLInsert(test_avl, (void *)(size_t)8);
    AVLInsert(test_avl, (void *)(size_t)10);

    actual_height = AVLGetHeight(test_avl);

    AVLForEach(test_avl, PrintData, (void *)&g_verbose); 

    if(actual_height != expected_height)
    {
        AVLDestroy(test_avl);
        printf("\t%sLine %d - Expected height %ld, actual height %ld\n",
                             RED, __LINE__ - 4, expected_height, actual_height);
        return 1;
    }

    AVLDestroy(test_avl); 
    return 0; 
}

static int TestBalance2() 
{
    avl_t *test_avl = AVLCreate(Compare, NULL);
    size_t expected_height = 1;
    size_t actual_height = 0;

    if(1 != AVLIsEmpty(test_avl))
    {
        AVLDestroy(test_avl);
        printf("\t%sLine %d - Expected empty tree\n", RED, __LINE__ - 3);
        return 1;
    }
        
    /* fill tree  - complex balancing (insert) */
    AVLInsert(test_avl, (void *)(size_t)4);
    AVLInsert(test_avl, (void *)(size_t)8);
    AVLInsert(test_avl, (void *)(size_t)6);

    actual_height = AVLGetHeight(test_avl);

    AVLForEach(test_avl, PrintData, (void *)&g_verbose); 

    if(actual_height != expected_height)
    {
        AVLDestroy(test_avl);
        printf("\t%sLine %d - Expected height %ld, actual height %ld\n",
                             RED, __LINE__ - 4, expected_height, actual_height);
        return 1;
    }

    AVLDestroy(test_avl); 
    return 0; 
}


static int TestBalance3() 
{
    avl_t *test_avl = AVLCreate(Compare, NULL);
    size_t expected_height = 1;
    size_t actual_height = 0;

    if(1 != AVLIsEmpty(test_avl))
    {
        AVLDestroy(test_avl);
        printf("\t%sLine %d - Expected empty tree\n", RED, __LINE__ - 3);
        return 1;
    }
        
    /* fill tree  - complex balancing (insert) */
    AVLInsert(test_avl, (void *)(size_t)4);
    AVLInsert(test_avl, (void *)(size_t)2);
    AVLInsert(test_avl, (void *)(size_t)8);
    AVLInsert(test_avl, (void *)(size_t)6);

    AVLRemove(test_avl, (void *)(size_t)2);

    actual_height = AVLGetHeight(test_avl);

    AVLForEach(test_avl, PrintData, (void *)&g_verbose); 

    if(actual_height != expected_height)
    {
        AVLDestroy(test_avl);
        printf("\t%sLine %d - Expected height %ld, actual height %ld\n",
                             RED, __LINE__ - 4, expected_height, actual_height);
        return 1;
    }

    AVLDestroy(test_avl); 
    return 0; 
}

static int TestStress() 
{
    avl_t *test_avl = AVLCreate(Compare, NULL);
    size_t tree_size = 50000;
    size_t *nums = (size_t *)calloc(tree_size, sizeof(size_t));
    size_t i = 0;   
    size_t count = tree_size;
    size_t curr_rand = 0;
    size_t curr_find = 0;
    size_t expected_size = tree_size;
    size_t actual_size = 0;
    int flag = 0;

    srand((unsigned int)time(NULL));

    while(0 < count)
    {
        curr_rand = rand() + 1;
        flag = 0;

        for(i = 0; tree_size > i; ++i)
        {
            if(nums[i] == curr_rand)
            {
                flag = 1;
            }
        }

        if(0 == flag)
        {
            nums[count - 1] = curr_rand;    
            --count;
        }
    }

    /* fill tree  - complex balancing (insert) */
    for(i = 0; tree_size > i; ++i) 
    {
        AVLInsert(test_avl, (void *)nums[i]);
    }

    /* find all nums in the tree */
    for(i = 0; tree_size > i + 1; ++i)
    {  
        curr_find = (size_t)AVLFind(test_avl, (void *)nums[i]);

        if(curr_find != nums[i])
        {
            free(nums);
            AVLDestroy(test_avl);
            printf("\t%sLine %d -couldn't find %ld\n",
                                                    RED, __LINE__ - 4, nums[i]);
            return 1;
        }
    }

    g_i = 0;

    /* fill nums back to array from tree (hopefully in order)*/
    AVLForEach(test_avl, PrintData, (void *)&g_verbose); 
    AVLForEach(test_avl, FillNums, (void *)nums); 

    for(i = 0; tree_size - 1 > i; ++i)
    {
        if(nums[i] >= nums[i + 1])
        {  
            free(nums);
            AVLDestroy(test_avl);
            printf("\t%sLine %d - %ld came before %ld in ''sorted'' tree\n",
                                       RED, __LINE__ - 4, nums[i], nums[i + 1]);
        }
    }

    actual_size = AVLGetSize(test_avl);

    if(actual_size != expected_size)
    {
        free(nums);
        AVLDestroy(test_avl);
        printf("\t%sLine %d - Expected size %ld, actual size %ld\n",
                                 RED, __LINE__ - 4, expected_size, actual_size);
        return 1;
    }

    free(nums);
    AVLDestroy(test_avl); 
    return 0; 
}

static int TestRemove()
{
    avl_t *test_avl = AVLCreate(Compare, NULL);
    size_t expected_nums[] = {2, 3, 4, 6, 7, 9};
    size_t i = 0;

    g_i = 0;

    if(1 != AVLIsEmpty(test_avl))
    {
        AVLDestroy(test_avl);
        printf("\t%sLine %d - Expected empty tree\n", RED, __LINE__ - 3);
        return 1;
    }
        
    /* fill tree */
    AVLInsert(test_avl, (void *)(size_t)5);
    AVLInsert(test_avl, (void *)(size_t)3);
    AVLInsert(test_avl, (void *)(size_t)7);
    AVLInsert(test_avl, (void *)(size_t)2);
    AVLInsert(test_avl, (void *)(size_t)4);
    AVLInsert(test_avl, (void *)(size_t)6);
    AVLInsert(test_avl, (void *)(size_t)9); 

    AVLRemove(test_avl, (void *)(size_t)5);

    AVLForEach(test_avl, PrintData, (void *)&g_verbose); 

    AVLForEach(test_avl, GetData, NULL); 

    for(; 6 > i + 1; ++i)
    {  
        if(g_nums[i] != expected_nums[i])
        {
            AVLDestroy(test_avl);
            printf("\t%sLine %d - Expected data %ld, actual data %ld\n",
                     RED, __LINE__ - 4, expected_nums[i], g_nums[i]);
            return 1;
        }
    }
    
    AVLDestroy(test_avl); 
    return 0;
}

static int TestForEach()
{
    avl_t *test_avl = AVLCreate(Compare, NULL);
    size_t expected_nums[] = {2, 3, 4, 5, 6, 7, 9};
    size_t i = 0;

    if(1 != AVLIsEmpty(test_avl))
    {
        AVLDestroy(test_avl);
        printf("\t%sLine %d - Expected empty tree\n", RED, __LINE__ - 3);
        return 1;
    }
        
    /* fill tree */
    AVLInsert(test_avl, (void *)(size_t)5);
    AVLInsert(test_avl, (void *)(size_t)3);
    AVLInsert(test_avl, (void *)(size_t)7);
    AVLInsert(test_avl, (void *)(size_t)2);
    AVLInsert(test_avl, (void *)(size_t)4);
    AVLInsert(test_avl, (void *)(size_t)6);
    AVLInsert(test_avl, (void *)(size_t)9); 

    AVLForEach(test_avl, PrintData, (void *)&g_verbose); 
    AVLForEach(test_avl, GetData, NULL);

    for(; 8 > i + 1; ++i)
    {  
        if(g_nums[i] != expected_nums[i])
        {
            AVLDestroy(test_avl);
            printf("\t%sLine %d - Expected data %ld, actual data %ld (i=%ld)\n",
                     RED, __LINE__ - 4, expected_nums[i], g_nums[i], i);
            return 1;
        }
    }

    AVLDestroy(test_avl);
    return 0;
}

static int TestCreateIsEmptyInsertHeight()
{
    avl_t *test_avl = AVLCreate(Compare, NULL);
    size_t expected_size = 7;
    size_t actual_size = 0;
    size_t expected_height = 2;
    size_t actual_height = 0;
    
    if(1 != AVLIsEmpty(test_avl))
    {
        AVLDestroy(test_avl);
        printf("\t%sLine %d - Expected empty tree\n", RED, __LINE__ - 3);
        return 1;
    }       
  
    AVLInsert(test_avl, (void *)(size_t)5);
    AVLInsert(test_avl, (void *)(size_t)3);
    AVLInsert(test_avl, (void *)(size_t)7);
    AVLInsert(test_avl, (void *)(size_t)2);
    AVLInsert(test_avl, (void *)(size_t)4);
    AVLInsert(test_avl, (void *)(size_t)6);
    AVLInsert(test_avl, (void *)(size_t)9); 

    
    actual_size = AVLGetSize(test_avl);

    if(actual_size != expected_size)
    {
        AVLDestroy(test_avl);
        printf("\t%sLine %d - Expected size %ld, actual size %ld\n",
                                 RED, __LINE__ - 4, expected_size, actual_size);
        return 1;
    }

    AVLForEach(test_avl, PrintData, (void *)&g_verbose); 

    actual_height = AVLGetHeight(test_avl);

    if(actual_height != expected_height)
    {
        AVLDestroy(test_avl);
        printf("\t%sLine %d - Expected height %ld, actual height %ld\n",
                             RED, __LINE__ - 4, expected_height, actual_height);
        return 1;
    }
    
    AVLDestroy(test_avl);
    return 0;
}

int main(int argc, char **argv)
{
    if(1 < argc)
    {
        g_verbose = atoi(argv[1]);   
    }

    RUN_TEST(TestCreateIsEmptyInsertHeight);
    RUN_TEST(TestForEach);
    RUN_TEST(TestFind);
    RUN_TEST(TestRemove);
    RUN_TEST(TestBalance1);
    RUN_TEST(TestBalance2);
    RUN_TEST(TestBalance3);
    RUN_TEST(TestStress);
    
    return 0;
}
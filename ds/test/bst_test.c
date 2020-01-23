#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "../include/bst.h" /* bst header file */

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

static int DoubleEach(void *data, const void *args)
{
    size_t *num = (size_t *)data;


    (void)args;
    
    *num *= 2;
/*    printf("%d\n", *num);*/

    return 0;
}

static int Compare(const void *d1, const void *d2, const void *params)
{
    (void)params;

    return (size_t)d1 - (size_t)d2;
}

int TestCreateIsEmptyInsert()
{
    bst_t *test_bst = BSTCreate(Compare, NULL);
    bst_it_t test_insert = NULL;  
    bst_it_t test_begin = NULL;  

    /* tree should be empty */
    if(1 != BSTIsEmpty(test_bst))
    {
        BSTDestroy(test_bst);
        printf("\t%sLine %d - IsEmpty returned 0 with empty BST\n",
                                                             RED, __LINE__ - 4);
        return 1;
    }

    test_insert = BSTInsert(test_bst, (void *)23);

    /* end should not be the same as the previously inserted node */
    if(BSTEnd(test_bst) == test_insert)
    {
        BSTDestroy(test_bst);
        printf("\t%sLine %d - Didn't insert new node into BST\n",
                                                             RED, __LINE__ - 4);
        return 1;
    }

    test_begin = BSTBegin(test_bst);

    /* begginning should be the same as the previously inserted node */
    if(test_begin != test_insert)
    {
        BSTDestroy(test_bst);
        printf("\t%sLine %d - Couldn't find inserted node\n",
                                                             RED, __LINE__ - 4);
        return 1;
    }

    BSTDestroy(test_bst);

    return 0;
}

int TestNext()
{
    bst_t *test_bst = BSTCreate(Compare, NULL);
    bst_it_t curr = NULL;  
    size_t i = 2300;
    size_t curr_data = 0;

    /* insert 20 - 50 */
    for(i = 20; i <= 50; i += 10)
    {
        BSTInsert(test_bst, (void *)i);
    }

    /* insert 1 - 19 */
    for(i = 1; i <= 19; ++i)
    {
        BSTInsert(test_bst, (void *)i);
    }

    curr = BSTBegin(test_bst);

    /* check if 1 - 20 is in the tree */
    for(i = 1; i < 20; ++i)
    {
        curr_data = (size_t)BSTGetData(curr);

        if(curr_data != i)
        {
            BSTDestroy(test_bst);
            printf("\t%sLine %d - Expected data %ld, actual data %ld (i=%ld)\n",
                                        RED, __LINE__ - 4, (i+1), curr_data, i);
            return 1;
        }
        
        curr = BSTNext(curr);
    }

    /* check is 20 - 50 is in the tree */
    for(; i <= 50; i += 10)
    {
        curr_data = (size_t)BSTGetData(curr);

        if(curr_data != i)
        {
            BSTDestroy(test_bst);
            printf("\t%sLine %d - Expected data %ld, actual data %ld (i=%ld)\n",
                                        RED, __LINE__ - 4, (i+1), curr_data, i);
            return 1;
        }
        
        curr = BSTNext(curr);
    }

    BSTDestroy(test_bst);
    return 0;
}

int TestPrev()
{
    bst_t *test_bst = BSTCreate(Compare, NULL);
    bst_it_t curr = NULL;  
    size_t i = 2300;
    size_t curr_data = 0;

    /* insert 20 - 50 */
    for(i = 20; i <= 50; i += 10)
    {
        BSTInsert(test_bst, (void *)i);
    }

    /* insert 1 - 19 */
    for(i = 1; i <= 19; ++i)
    {
        BSTInsert(test_bst, (void *)i);
    }

    curr = BSTPrev(BSTEnd(test_bst));

    /* run from the end, check if 50 - 20 is in the tree */
    for(i = 50; i >= 20; i -= 10)
    {
        curr_data = (size_t)BSTGetData(curr);

        if(curr_data != i)
        {
            BSTDestroy(test_bst);
            printf("\t%sLine %d - Expected data %ld, actual data %ld (i=%ld)\n",
                                        RED, __LINE__ - 4, (i+1), curr_data, i);
            return 1;
        }
        
        curr = BSTPrev(curr);
    }

    /* keep running backwards, check if 19 - 1 is in the tree */
    for(i = 19; i >= 1; --i)
    {
        curr_data = (size_t)BSTGetData(curr);

        if(curr_data != i)
        {
            BSTDestroy(test_bst);
            printf("\t%sLine %d - Expected data %ld, actual data %ld (i=%ld)\n",
                                        RED, __LINE__ - 4, (i+1), curr_data, i);
            return 1;
        }
        
        if(1 < i)
        {
            curr = BSTPrev(curr);
        }
    }

    BSTDestroy(test_bst);
    return 0;
}

int TestCount()
{
    bst_t *test_bst = BSTCreate(Compare, NULL);  
    size_t i = 0;
    size_t expected_count = 23;
    size_t actual_count = 0;

    /* insert 20 - 50 */
    for(i = 20; i <= 50; i += 10)
    {
        BSTInsert(test_bst, (void *)i);
    }

    /* insert 1 - 19 */
    for(i = 1; i <= 19; ++i)
    {
        BSTInsert(test_bst, (void *)i);
    }

    actual_count = BSTCount(test_bst);
    
    if(actual_count != expected_count)
    {
        BSTDestroy(test_bst);
        printf("\t%sLine %d - Expected count %ld, actual count %ld\n",
                               RED, __LINE__ - 4, expected_count, actual_count);
        return 1;
    }

    BSTDestroy(test_bst);
    return 0;
}

int TestFind()
{
    bst_t *test_bst = BSTCreate(Compare, NULL);
    size_t end_data = 0xDEADBEEF;
    size_t i = 2300;
    size_t curr_data = 0;

    /* insert 20 - 50 */
    for(i = 20; i <= 50; i += 10)
    {
        BSTInsert(test_bst, (void *)i);
    }

    /* insert 1 - 19 */
    for(i = 1; i <= 19; ++i)
    {
        BSTInsert(test_bst, (void *)i);
    }

    /* check if 20 - 50 is in the tree */
    for(i = 20; i <= 50; i += 10)
    {
        curr_data = (size_t)BSTGetData(BSTFind(test_bst, (void *)i));

        if(curr_data == end_data)
        {
            BSTDestroy(test_bst);
            printf("\t%sLine %d - couldn't find data %ld\n",
                                        RED, __LINE__ - 4, i);
            return 1;
        }
    }

    /* check if 1 - 19 is in the tree */
    for(i = 1; i < 20; ++i)
    {
        curr_data = (size_t)BSTGetData(BSTFind(test_bst, (void *)i));

        if(curr_data == end_data)
        {
            BSTDestroy(test_bst);
            printf("\t%sLine %d - couldn't find data %ld\n",
                                        RED, __LINE__ - 4, i);
            return 1;
        }

    }

    BSTDestroy(test_bst);
    return 0;
}

int TestForEach()
{
    bst_t *test_bst = BSTCreate(Compare, NULL);
    bst_it_t curr = NULL;
    size_t i = 2300;
    size_t curr_data = 0;

    size_t *arr = (size_t *)malloc(sizeof(size_t) * 20);
    if(NULL == arr)
    {
        return 1;
    }

    /* insert 1 - 20 */
    for(i = 1; i <= 20; ++i)
    {
        arr[i - 1] = i;
        BSTInsert(test_bst, (void *)(arr + i - 1));
    }

    /* double all data */
    BSTForEach(BSTBegin(test_bst), BSTEnd(test_bst), DoubleEach, NULL);

    /*start from the beginning */
    curr = BSTBegin(test_bst);

    /* check if 2 - 40 is in the tree */
    for(i = 1; i <= 20; ++i)
    {
        curr_data = *(size_t *)BSTGetData(curr);

        if(curr_data != (i * 2))
        {
            BSTDestroy(test_bst);
            printf("\t%sLine %d - Expected data %ld, actual data %ld (i=%ld)\n",
                                    RED, __LINE__ - 4, (i+1), curr_data, i * 2);
            return 1;
        }

        curr = BSTNext(curr);

    }

    free(arr);
    BSTDestroy(test_bst);
    return 0;
}

int TestRemove()
{
    bst_t *test_bst = BSTCreate(Compare, NULL);
    void *remove_data = (void *)13;
    bst_it_t curr = NULL;   
    size_t i = 2300;
    size_t curr_data = 0;

    /* insert 20 - 50 */
    for(i = 20; i <= 50; i += 10)
    {
        BSTInsert(test_bst, (void *)i);
    }

    /* insert 1 - 19 */
    for(i = 1; i <= 19; ++i)
    {
        BSTInsert(test_bst, (void *)i);
    }

    /* remove 13 from the list */
    BSTRemove(BSTFind(test_bst, remove_data));

    if(BSTFind(test_bst, remove_data) != NULL)
    {
            BSTDestroy(test_bst);
            printf("\t%sLine %d - couldn't remove %ld\n",
                                       RED, __LINE__ - 4, (size_t)remove_data);
            return 1;
    }    

    curr = BSTBegin(test_bst);

    /* check if 1 - 20 is in the tree */
    for(i = 1; i <= 20; ++i)
    {
        curr_data = (size_t)BSTGetData(curr);


        /* check if i is in the tree, ignore removed value, since it's no longer
        in the tree */
        if(curr_data != i && i != (size_t)remove_data)
        {
            BSTDestroy(test_bst);
            printf("\t%sLine %d - Expected data %ld, actual data %ld (i=%ld)\n",
                                        RED, __LINE__ - 4, (i+1), curr_data, i);
            return 1;
        }
        
        /* advance if i is not the removed value */
        if(i != (size_t)remove_data)
        {
            curr = BSTNext(curr); 
        }
    }

    BSTDestroy(test_bst);
    return 0;
}

int main()
{
    RUN_TEST(TestCreateIsEmptyInsert);
    RUN_TEST(TestNext);
    RUN_TEST(TestPrev);
    RUN_TEST(TestCount);
    RUN_TEST(TestFind);
    RUN_TEST(TestForEach);    
    RUN_TEST(TestRemove);

    return 0;
}

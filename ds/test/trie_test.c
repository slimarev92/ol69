/***************************************
* DS:          TRIE (TESTS)            *
* CODE BY:     SASHA LIMAREV           *   
* REVIEWED BY: SOMEONE                 *                          
****************************************/
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <limits.h> /* UCHAR_MAX */

#include "trie.h"

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
    \
    if(0 == test_name()) \
    { \
        printf("%s[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \
    } \
    printf("%s\n", RESET_COLOR);

static int TestCreateDestroy()
{
    trie_t *test_trie = TrieCreate(32);

    TrieDestroy(test_trie);

    return 0;
}

static int TestInsertExact()
{
    trie_t *test_trie = TrieCreate(32);
    unsigned int test = 23;
    int status = 0;

    status = TrieInsertExact(test_trie, (void *)&test);

    status = TrieInsertExact(test_trie, (void *)&test);


    if(NADDED_EXIST != status)
    {
        TrieDestroy(test_trie);
        printf("\t%sLine %d - added the same value twice\n", RED, __LINE__ - 3);   
        return 1;
    }

    test = 24;

    status = TrieInsertExact(test_trie, (void *)&test);

    if(ADDED != status)
    {
        TrieDestroy(test_trie);
        printf("\t%sLine %d - couldn't add new value \n", RED, __LINE__ - 3);   
        return 1;
    }

    TrieDestroy(test_trie);

    return 0;
}

static int TestInsertClose()
{
    trie_t *test_trie = TrieCreate(8);
    unsigned int test = 23;
    unsigned int expected = 22;
    unsigned int actual = 0;
    int status = 0;

    status = TrieInsertExact(test_trie, (void *)&test);
/*    printf("----------------------\n");*/
    status = TrieInsertClose(test_trie,  (void *)&test, (void *)&actual);
/*    printf("----------------------\n");*/

    if(ADDED_EXIST != status)
    {
        TrieDestroy(test_trie);
        printf("\t%sLine %d - couldn't add closest value\n", RED, __LINE__ - 3);   
        return 1;
    }

    if(expected != actual)
    {
        TrieDestroy(test_trie);
        printf("\t%sLine %d - Expected value %u, actual velue %u\n",
                               RED, __LINE__ - 4, expected, actual);
        return 1;

    }


    TrieDestroy(test_trie);

    return 0;
}

int TestCount()
{
    trie_t *test_trie = TrieCreate(8);
    unsigned int i = 0;
    size_t expected_count = UCHAR_MAX - 2;
    size_t actual_count = 0;
    int status = 0;
    unsigned char curr = 0;

    for(; UCHAR_MAX >= i; ++i)
    {
        curr = (unsigned char)i;
        /*printf("adding %u\n", i);*/
        status = TrieInsertExact(test_trie, (void *)&curr);

        if(ADDED != status)
        {
            TrieDestroy(test_trie);
            printf("\t%sLine %d - couldn't add new value (i = %u)\n",
                                                          RED, __LINE__ - 3, i);   
            return 1;
        }

    }

    /* remove 10 */
    curr = 10;
    TrieRemove(test_trie, (void *)&curr);

    /* remove 20 */
    curr = 20;
    TrieRemove(test_trie, (void *)&curr);

    /* remove 30 */
    curr = 30;
    TrieRemove(test_trie, (void *)&curr);

    /* attempt to remove 30 again */
    curr = 30;
    status = TrieRemove(test_trie, (void *)&curr);

    if(REMOVED == status)
    {
        TrieDestroy(test_trie);
        printf("\t%sLine %d - Removed same value twice\n", RED, __LINE__ - 3);
        return 1;
    }

    actual_count =  TrieCount(test_trie);

    /* check if free count matched expected value */
    if(actual_count != expected_count)
    {
        TrieDestroy(test_trie);
        printf("\t%sLine %d - Expected count %ld, actual count %ld\n",
                               RED, __LINE__ - 4, expected_count, actual_count);
        return 1;

    }

     TrieDestroy(test_trie);

    return 0;
}

int main()
{
    RUN_TEST(TestCreateDestroy);
    RUN_TEST(TestInsertExact);
    RUN_TEST(TestCount);
    RUN_TEST(TestInsertClose);

    return 0;
}

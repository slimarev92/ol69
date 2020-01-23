#include <stdio.h> /* fopen */
#include <stdlib.h> /* malloc */
#include <time.h> /* time */
#include <string.h> /* strlen */

#include "hash.h" /* hash table header file */

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[93m"
#define RESET_COLOR "\x1b[0m"

#define HASH_RANGE (729)
#define WORD_HASH_RANGE (72)
#define NEW_BALANCE (23.23)
#define DICT_SIZE (99171)

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

/* bank account struct */
typedef struct account
{
    size_t account_num;
    double balance;
} account_t;


/* sets account balance to zero */
int PrintWord(void *data, const void *params)
{
    (void)params;

    printf("%s", (char *)data);

    return 0;
}

/* sets account balance to zero */
int ChangeBalance(void *data, const void *params)
{
    (void)params;

    ((account_t *)data)->balance = NEW_BALANCE;

    return 0;
}

static int CompareWords(const void *data, const void *target)
{
    /* printf("\tbalance: %.3f\n", ((account_t *)data)->balance); */

    return !strcmp((char *)data, (char *)target);;
}

/* hash is strlen plus ascii value of first letter - range is 72*/
static size_t WordHash(void *data)
{
    char *word = (char *)data;

    return strlen(word) + (size_t)*word - (size_t)'a';
}

/* compares account numbers - returns one if they're the same, zero otherwise */
static int Compare(const void *data, const void *target)
{
    /* printf("\tbalance: %.3f\n", ((account_t *)data)->balance); */

    return ((account_t *)data)->account_num == 
                                             ((account_t *)target)->account_num;
}

static char **ReadDict()
{
    char **words = NULL;
    FILE *dict = NULL;
    size_t i = 0;
    
    words = calloc(sizeof(char **), DICT_SIZE);
    if(NULL == words)
    {
        return NULL;
    }

    dict = fopen("/usr/share/dict/words", "r");
    if(NULL == dict)
    {
        free(words);
        return NULL;
    }

    for(; i < DICT_SIZE; ++i)
    {
        words[i] = (char *)calloc(sizeof(char *), 46);
        fgets(words[i], 46, dict); 
    }

    fclose(dict);

    return words;
}

void DestroyWords(char **words)
{
    size_t i = 0;

    for(; i < DICT_SIZE; ++i)
    {
        free(words[i]);
    }

    free(words);
}

static int TestDict()
{
    char **words = ReadDict();
    hash_t *test_table = HashCreate(CompareWords, WordHash, WORD_HASH_RANGE);
    size_t i = 0;

    for(; i < DICT_SIZE - 20; ++i)
    {
        HashInsert(test_table, (void *)words[i]);
    }
    
    /* HashForEach(test_table,  PrintWord, NULL); */

    for(i = 0; i < WORD_HASH_RANGE - 20; ++i)
    {      
        if((char *)HashFind(test_table, (void *)words[i]) != words[i])
        {
            HashDestroy(test_table);
            printf("\t%sLine %d - Couldn't find word %s (i=%ld)\n",
                      RED, __LINE__ - 4, words[i], i);

            return 1;
        }
    }
    
    HashDestroy(test_table);
    DestroyWords(words);
    return 0;
}

/* product of last three digits of account number - range is 729 */
static size_t HashFunction(void *data)
{
    size_t account_num = ((account_t *)data)->account_num;  
    size_t hash =  (account_num % 10) * ((account_num % 100) / 10) 
                                               * ((account_num % 1000) / 100);

    return hash;
}

static int TestCreateDestroy()
{
    hash_t *test_table = HashCreate(Compare, HashFunction, HASH_RANGE);

    HashDestroy(test_table);

    return 0;
}

static int TestForEach()
{
    hash_t *test_table = HashCreate(Compare, HashFunction, HASH_RANGE);
    account_t *curr_account = NULL;
    static account_t accounts[] = {{1112, 400.16},
                                   {1111, 100.0}, 
                                   {2211, -23.20}, 
                                   {1234, 555.0}, 
                                   {3333, 9999.99},
                                   {9911, -200.0}};
    size_t i = 0;
                                 
    for(; 6 > i; ++i)
    {
        HashInsert(test_table, (void *)&accounts[i]);
    }

    HashForEach(test_table,  ChangeBalance, NULL);

    for(i = 0; 6 > i; ++i)
    {

        if(NEW_BALANCE != accounts[i].balance)
        {
            HashDestroy(test_table);
            printf("\t%sLine %d - Exp balance %.3f Act balance %.3f (i=%ld)\n",
                      RED, __LINE__ - 4, NEW_BALANCE, curr_account->balance, i);

            return 1;
        }
    }

    HashDestroy(test_table);
    return 0;
}

static int TestInsertFind()
{
    hash_t *test_table = HashCreate(Compare, HashFunction, HASH_RANGE);
    account_t *curr_account = NULL;
    size_t expected_size = 6;
    size_t actual_size = 0;
    static account_t accounts[] = {{1112, 400.16},
                                   {1111, 100.0}, 
                                   {2211, -23.20}, 
                                   {1234, 555.0}, 
                                   {3333, 9999.99},
                                   {9911, -200.0}};
    size_t i = 0;
    account_t target_account = {1234, 999};
    static double expected_balance[] = {400.16, 100.0, -23.20, 555.0, 
                                                               9999.99, -200.0};
          
    if(1 != HashIsEmpty(test_table))
    {
        HashDestroy(test_table);
        printf("\t%sLine %d - HashIsEmpty returned 0 on empty table\n",
                                                             RED, __LINE__ - 4);

        return 1;
    }
                                               
    for(; 6 > i; ++i)
    {
        HashInsert(test_table, (void *)&accounts[i]);
    }

    actual_size = HashSize(test_table);

    if(actual_size != expected_size)
    {
        HashDestroy(test_table);
        printf("\t%sLine %d - Expected Size %ld, Actual size %ld (i=%ld)\n",
                             RED, __LINE__ - 4, expected_size, actual_size, i);

        return 1;
    }
    
    for(i = 0; 6 > i; ++i)
    {
        target_account.account_num = accounts[i].account_num;
        curr_account = HashFind(test_table, (void *)&target_account);

        if(NULL == curr_account || curr_account->balance != expected_balance[i])
        {
            HashDestroy(test_table);
            printf("\t%sLine %d - Couldn't find account %ld in table (i=%ld)\n",
                             RED, __LINE__ - 4, target_account.account_num , i);

            return 1;
        }
    }

    HashDestroy(test_table);
    return 0;
}

static int TestRemove()
{
    hash_t *test_table = HashCreate(Compare, HashFunction, HASH_RANGE);
    account_t *curr_account = NULL;
    size_t expected_size = 4;
    size_t actual_size = 0;
    static account_t accounts[] = {{1112, 400.16},
                                   {1111, 100.0}, 
                                   {2211, -23.20}, 
                                   {1234, 555.0}, 
                                   {3333, 9999.99},
                                   {9911, -200.0}};
    size_t i = 0;
    account_t target_account = {1234, 999};
    static double expected_balance[] = {400.16, 100.0, -23.20, 555.0, 
                                                               9999.99, -200.0};
                                                          
    for(; 6 > i; ++i)
    {
        HashInsert(test_table, (void *)&accounts[i]);
    }
  
    /* remove account 1112 */
    target_account.account_num = accounts[0].account_num;
    HashRemove(test_table, (void *)&target_account);

    /* remove account 1111 */
    target_account.account_num = accounts[1].account_num;
    HashRemove(test_table, (void *)&target_account);


    actual_size = HashSize(test_table);

    if(actual_size != expected_size)
    {
        HashDestroy(test_table);
        printf("\t%sLine %d - Expected Size %ld, Actual size %ld (i=%ld)\n",
                             RED, __LINE__ - 4, expected_size, actual_size, i);

        return 1;
    }
    
    for(i = 2; 6 > i; ++i)
    {
        target_account.account_num = accounts[i].account_num;
        curr_account = HashFind(test_table, (void *)&target_account);

        if(NULL == curr_account || curr_account->balance != expected_balance[i])
        {
            HashDestroy(test_table);
            printf("\t%sLine %d - Couldn't find account %ld in table (i=%ld)\n",
                             RED, __LINE__ - 4, target_account.account_num , i);

            return 1;
        }
    }

    HashDestroy(test_table);
    return 0;
}

int main()
{
    RUN_TEST(TestCreateDestroy);
    RUN_TEST(TestInsertFind);
    RUN_TEST(TestRemove);
    RUN_TEST(TestForEach);
    RUN_TEST(TestDict);
    
    return 0;
}
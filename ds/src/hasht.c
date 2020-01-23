/***************************************
* DATA STRUCTURES: HASH TABLE          *
* CODE BY:         SASHA LIMAREV       *   
* REVIEWED BY:     SOMEONE             *                          
****************************************/ 
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <math.h> /* pow */

#include "hash.h" /* hash table header file */        
#include "dll.h" /* dll header file */

struct hash
{
    int (*Compare)(const void *data, const void *key);
    size_t (*HashFunction)(void *key);
    size_t range;
    dll_t **table; 
};

static it_t FindInBucket(hash_t *hasht, dll_t *dll, void *data);
static size_t GetHash(hash_t *table, void *data);

hash_t *HashCreate(int (*Compare)(const void *data, const void *key),
                   size_t (*HashFunction)(void *key),
                   size_t range)
{
    size_t i = 0, j = 0;
    hash_t *new_table = NULL;

    assert(NULL != Compare);
    assert(NULL != HashFunction);
    assert(0 < range);

    new_table = (hash_t *)malloc(sizeof(*new_table));
    if(NULL == new_table)
    {
        return NULL;
    }

    new_table->Compare = Compare;
    new_table->HashFunction = HashFunction;
    new_table->range = range;
    new_table->table = (dll_t **)malloc(sizeof(*new_table->table) * range);
    if(NULL == new_table->table)
    {
        free(new_table);
        return NULL;
    }

    for(; i < range; ++i)
    {
        new_table->table[i] = DLLCreate();
        if(NULL == new_table->table[i])
        {
            for(j = 0; j < i; ++j)
            {
                DLLDestroy(new_table->table[i]);
            }

            return NULL;
        }
    }

    return new_table;
}

void HashDestroy(hash_t *table)
{
    size_t i = 0;

    for(; i < table->range; ++i)
    {
        DLLDestroy(table->table[i]);
        table->table[i] = NULL;
    }

    free(table->table);
    free(table);
}

int HashInsert(hash_t *table, void *data)
{
    size_t hash = GetHash(table, data);

    it_t new_node = DLLPushBack(table->table[hash], data); 

    return (DLLEnd(table->table[hash]) != new_node) ? 0 : 1;
}

void HashRemove(hash_t *table, void *target)
{
    size_t hash = 0;
    dll_t *bucket = NULL;
    it_t found = NULL;
    it_t end = NULL;

    assert(NULL != table);
    assert(NULL != target);

    hash = GetHash((hash_t *)table, target);
    bucket = table->table[hash];
    found = FindInBucket((hash_t *)table, bucket, target);
    end = DLLEnd(bucket);

    if(!DLLIsSameIter(found, end))
    {
        DLLErase(found);
    }
}

void *HashFind(const hash_t *table, void *target)
{
    size_t hash = 0;
    dll_t *bucket = NULL;
    it_t found = NULL;
    it_t end = NULL;

    assert(NULL != table);
    assert(NULL != target);

    hash = GetHash((hash_t *)table, target);
    bucket = table->table[hash];
    end = DLLEnd(bucket);
    found = FindInBucket((hash_t *)table, bucket, target);

    return (!DLLIsSameIter(found, end)) ? DLLGetData(found) : NULL;
}

static size_t GetHash(hash_t *table, void *data)
{
    return table->HashFunction(data) % table->range;
}

static it_t FindInBucket(hash_t *table, dll_t *bucket, void *data)
{
    it_t curr = NULL;
    it_t end = DLLEnd(bucket);

    for(curr = DLLBegin(bucket); !DLLIsSameIter(curr, end); 
                                                          curr = DLLNext(curr))
    {
        if(0 != table->Compare(DLLGetData(curr), data))
        {
            return curr;
        }
    }

    return NULL;
}

size_t HashSize(const hash_t *table)
{
    size_t i = 0;
    dll_t *curr_bucket = 0;
    size_t size = 0;

    for(; i < table->range; ++i)
    {
        curr_bucket = table->table[i];
        size += DLLSize(curr_bucket);
    }

    return size;
}

int HashIsEmpty(const hash_t *table)
{
    size_t i = 0;

    assert(NULL != table);

    for(; i < table->range; ++i)
    {
        if(!DLLIsEmpty(table->table[i]))
        {
            return 0;
        }
    }

    return 1;
}

int HashForEach(hash_t *table, 
                int (*Action)(void *data, const void *params),
                const void *params)
{
    dll_t *bucket = NULL;
    size_t i = 0;
    size_t status = 0;

    assert(NULL != table);
    assert(NULL != Action);

    for(; i < table->range && 0 == status; ++i)
    {
        bucket = table->table[i];

        status = DLLForEach(DLLBegin(bucket), DLLEnd(bucket), Action, params);
    }

    return status;
}

double HashSD(const hash_t *table)
{
    double average = HashLoad(table);
    size_t i = 0;
    double sum_dev = 0;

    for(; i < table->range; ++i)
    {
        sum_dev += pow(DLLSize(table->table[i]) - average ,2.0);
    }

    return sqrt(sum_dev / table->range);
}

double HashLoad(const hash_t *table)
{
    int k = table->range;
    int n = HashSize(table);

    return (double)n / k;
}
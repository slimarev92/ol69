/***************************************
* SYSTEM PROGRAMMING: VSMA             *
* CODE BY:         SASHA LIMAREV       *   
* REVIEWED BY:     YAEL LOTAN          *                          
****************************************/ 
#include <sys/types.h> /* ssizet */
#include <assert.h> /* assert */
#include <stdlib.h> /* labs */

#include "vsma.h" /* VSMA header file */

#define WORD (sizeof(size_t))

struct vsma 
{
    void *pool;
};

typedef struct blk_header
{
    ssize_t block_size;
    #ifndef NDEBUG
    size_t magic_num;
    #endif
} blk_header_t;

static const size_t g_magic_num = 0xFEEFEE;
static const ssize_t g_end_num = 0xDEADBEEF;

static void VSAMInitBlock(void *block, ssize_t block_size);
static blk_header_t *VSMANext(blk_header_t *blk);
static int VSMAIsEnd(blk_header_t *blk);
static int VSMAIsAllocated(blk_header_t *blk);
static blk_header_t *VSMACombineBlocks(blk_header_t *left, blk_header_t *right);
static void *VSMAGetData(blk_header_t *blk);
static int VSMACanCombine(blk_header_t *curr, blk_header_t *next);

size_t VSMAFindLargestFree(vsma_t *vsma)
{
    blk_header_t *curr = NULL;
    blk_header_t *next = NULL;
    ssize_t largest = 0;

    assert(NULL != vsma);

    curr = (blk_header_t *)vsma;
    next = VSMANext(curr);

    while(!VSMAIsEnd(curr))
    {
        if(VSMACanCombine(curr, next))
        {
            curr = VSMACombineBlocks(curr, next);
            next = VSMANext(curr);
        }
        else if(!VSMAIsAllocated(curr))
        {
            largest = curr->block_size > largest ? curr->block_size : largest;
            curr = next;
            next = VSMANext(curr);
        }
        else
        {
            curr = next;
            next = VSMANext(curr);
        }
    }

    return largest;
}

size_t VSMACount(vsma_t *vsma)
{
    size_t counter = 0;
    blk_header_t *curr = NULL;
    blk_header_t *next = NULL;

    assert(NULL != vsma);

    counter = 0;
    curr = (blk_header_t *)vsma;
    next = VSMANext(curr);

    while(!VSMAIsEnd(curr))
    {
        if(VSMACanCombine(curr, next))
        {
            curr = VSMACombineBlocks(curr, next);
            next = VSMANext(curr);
        }
        else if(!VSMAIsAllocated(curr))
        {
            counter += curr->block_size;
            curr = next;
            next = VSMANext(curr);
        }
        else
        {
            curr = next;
            next = VSMANext(curr);
        }
    }

    return counter;
}

void VSMAFree(void *block)
{
    blk_header_t *blk = NULL;

    assert(NULL != block);

    blk = (blk_header_t *)((size_t)block - sizeof(blk_header_t));

    assert(blk->magic_num == g_magic_num);

    blk->block_size *= -1;

    return;
}

void *VSMAAlloc(vsma_t *vsma, size_t block_size)
{
    blk_header_t *curr = NULL;
    blk_header_t* next = NULL;
    size_t next_size = 0;
    ssize_t desired_size = 0;

    assert(NULL != vsma);
    assert(0 != block_size);

    curr = (blk_header_t *)vsma;
    next = VSMANext(curr);
    desired_size = 0 == block_size % WORD ? block_size : 
                     block_size + (WORD - (block_size % WORD));

    while(!VSMAIsEnd(curr))
    {
        if(VSMACanCombine(curr, next))
        {
            curr = VSMACombineBlocks(curr, next);
            next = VSMANext(curr);
        }
        else if(!VSMAIsAllocated(curr) && curr->block_size >= desired_size)
        {
            break;
        }
        else 
        {
            curr = next;
            next =  VSMANext(curr);
        }
    }
    
    if(!VSMAIsEnd(curr))
    {
        next_size = curr->block_size  - desired_size;

        if(next_size >= WORD + sizeof(blk_header_t))
        {
            curr->block_size = desired_size * -1;

            VSAMInitBlock((void *)((size_t)curr + desired_size +
                      sizeof(blk_header_t)), next_size - sizeof(blk_header_t));
        }
        else
        {
            curr->block_size *= -1;
        }

        return VSMAGetData(curr);
    }

    return NULL;
}

vsma_t *VSMAInit(void *pool, size_t pool_size)
{
    size_t *end =  NULL;
    size_t adjusted_pool = pool_size - sizeof(blk_header_t);
    size_t end_location = adjusted_pool - (adjusted_pool % WORD);

    assert(NULL != pool);
    assert(0 != pool_size);

    VSAMInitBlock(pool, end_location - sizeof(blk_header_t));
  
    end = (size_t *)((size_t)pool + end_location);    
    *end = g_end_num;

    return (vsma_t *)pool;
}

static void VSAMInitBlock(void *block, ssize_t block_size)
{
    blk_header_t *header = NULL;

    assert(NULL != block);
    assert(0 != block_size);

    header = (blk_header_t *)block;

    header->block_size = block_size;

    #ifndef NDEBUG
    header->magic_num = g_magic_num;
    #endif  
}

static void *VSMAGetData(blk_header_t *blk)
{
    return (void *)((size_t)blk + sizeof(blk_header_t));
}

static int VSMAIsEnd(blk_header_t *blk)
{
    return g_end_num == blk->block_size;
}

static int VSMAIsAllocated(blk_header_t *blk)
{
    assert(NULL != blk);

    return 0 > blk->block_size;
}

static blk_header_t *VSMACombineBlocks(blk_header_t *left, blk_header_t *right)
{
    assert(NULL != left && NULL != right);

    left->block_size += right->block_size + sizeof(blk_header_t);

    return left;
}

static int VSMACanCombine(blk_header_t *curr, blk_header_t *next)
{
   return !VSMAIsAllocated(curr) && !VSMAIsEnd(next) && !VSMAIsAllocated(next);
}

static blk_header_t *VSMANext(blk_header_t *blk)
{
    long jump = 0;

    assert(NULL != blk);

    jump =  labs(blk->block_size);
    
    return (blk_header_t *)((size_t)blk + jump + sizeof(blk_header_t));
}

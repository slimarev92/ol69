/****************************************************
* SYSTEM PROGRAMMING: FIXED SIZE MEMEORY ALLOCATOR  *
* CODE BY:            SASHA LIMAREV                 *   
* REVIEWED BY:        MISHEL LIBERMAN               *                          
*****************************************************/ 
#include <assert.h> /* assert */

#include "fsma.h" /* FSMA library header */

#define WORD sizeof(size_t)
#define META_SIZE sizeof(size_t)
#define FSMA_SIZE sizeof(fsma_t)

struct fsma
{
	size_t next_free;
	size_t block_size;
	size_t n_blocks;
};

size_t FSMASuggestSize(size_t n_blocks, size_t block_size)
{
    size_t optimal_block_size = 0 == (block_size % WORD) ? 
                   block_size : (block_size / WORD + 1) * WORD;

    return FSMA_SIZE + (optimal_block_size + META_SIZE) * n_blocks;
}

void FSMAFree(void *block)
{
    size_t *block_meta = (size_t *)((size_t)block - META_SIZE);
    fsma_t *fsma = (fsma_t *)((size_t)block_meta - *block_meta);

    assert(NULL != block);

    block_meta = (size_t *)((size_t)block - META_SIZE);
    fsma = (fsma_t *)((size_t)block_meta - *block_meta);

    *block_meta = fsma->next_free;
   
    fsma->next_free = (size_t)block - (size_t)fsma;
}

void *FSMAAlloc(fsma_t *fsma)
{
    void *retval = NULL;
    size_t *next_meta = NULL;

    assert(NULL != fsma);

    if(0 < fsma->next_free)
    {
        retval = (void *)((size_t)fsma + fsma->next_free);
        next_meta = (size_t *)((size_t)retval - META_SIZE);

        fsma->next_free = *next_meta;   
        *next_meta = (size_t)next_meta - (size_t)fsma;
    }

    return retval;
}

fsma_t *FSMAInit(void *pool, size_t pool_size, size_t block_size)
{
    fsma_t *fsma = (fsma_t *)pool;

    size_t next_offset = 0;
    size_t *curr_meta = NULL;
    size_t i = 0;
    size_t max_iters = 0;
    size_t iter_offset = 0;

    assert(NULL != pool && 0 < pool_size && 0 < block_size);

    fsma->next_free = FSMA_SIZE + META_SIZE;
    fsma->block_size = 0 == (block_size % WORD) ? 
                            block_size : (block_size / WORD + 1) * WORD;
    fsma->n_blocks =  (pool_size - FSMA_SIZE) / (fsma->block_size + META_SIZE);

    iter_offset = fsma->block_size + META_SIZE;
    next_offset = fsma->next_free +  iter_offset;   
    curr_meta = (size_t *)((size_t)pool + fsma->next_free - META_SIZE);
    max_iters = fsma->n_blocks - 1;

    for(; i < max_iters; ++i)
    {
        *curr_meta = next_offset;
        curr_meta = (size_t *)((size_t)curr_meta + iter_offset);
        next_offset += iter_offset;
    }

    *curr_meta = 0;

    return fsma;
}

size_t FSMACountFree(fsma_t *fsma)
{
    size_t next_free = 0;
    size_t *curr_meta = NULL;
    size_t counter = 0;

    assert(NULL != fsma);

    next_free = fsma->next_free;
    curr_meta = (size_t *)((size_t)fsma + next_free - META_SIZE);

    while(0 != next_free)
    {
        ++counter;
        next_free = *curr_meta;
        curr_meta = (size_t *)((size_t)fsma + next_free - META_SIZE);
    }    
    
    return counter;
}

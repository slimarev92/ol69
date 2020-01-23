#ifndef VSMA_H
#define VSMA_H

#include <stddef.h>

typedef struct vsma vsma_t;

/* Initiates allocation manager */
vsma_t *VSMAInit(void *pool, size_t pool_size);

/* Allocates memory block */
void *VSMAAlloc(vsma_t *vsma, size_t block_size);

/* Frees memory block */
void VSMAFree(void *block);

/* Returns amount of blocks free for writing */
size_t VSMACount(vsma_t *vsma);

/* Returns size of largest free chunk */
size_t VSMAFindLargestFree(vsma_t *vsma);

#endif

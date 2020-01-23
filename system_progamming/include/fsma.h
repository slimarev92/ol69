#ifndef FSMA_H
#define FSMA_H

#include <stddef.h>

typedef struct fsma fsma_t;

size_t FSMASuggestSize(size_t n_blocks, size_t block_size);

fsma_t *FSMAInit(void *pool, size_t pool_size, size_t block_size);

void *FSMAAlloc(fsma_t *fsma);

void FSMAFree(void *block);

size_t FSMACountFree(fsma_t *fsma);

#endif

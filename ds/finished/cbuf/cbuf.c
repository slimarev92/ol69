/**********************************
* DATA STRUCTURES: CYCLIC BUFFER  *
* CODE BY:         SASHA LIMAREV  *   
* REVIEWED BY:     ROI KRAMER     *                          
***********************************/ 

#include <stdlib.h> /* malloc */
#include <errno.h> /* errno */

#include "cbuf.h"

static char *GetWritePtr(cbuf_t *cbuf, int i);

struct CBUF
{
    size_t capacity;
    size_t size;
    size_t read_offset;
    void *buff;
};

int CBUFIsEmpty(const cbuf_t *cbuf)
{
    return (0 == cbuf->size);
}

size_t CBUFWrite(cbuf_t *cbuf, const void *data, size_t nbytes)
{
    size_t i = 0;
    char *write_ptr = NULL;
    char *data_ptr = (char *)data;

    if(nbytes > CBUFFreeSpace(cbuf))
    {
        nbytes = CBUFFreeSpace(cbuf);
        errno = EOVERFLOW;
    }

    for(; i < nbytes; ++i)
    {
        write_ptr = GetWritePtr(cbuf, i);
        *write_ptr = *(data_ptr + i);
    }

    cbuf->size += i;

    return i;
}

size_t CBUFRead(cbuf_t *cbuf, void *data, size_t nbytes)
{
    size_t i = 0;
    char *data_ptr = (char *)data;
    char *read_ptr = (char *)cbuf->buff;

    if(nbytes >= cbuf->size)
    {
        nbytes = cbuf->size;
        errno = ENODATA;
    }
    
    for(; i < nbytes; ++i)
    {
        *(data_ptr + i) = *(read_ptr + cbuf->read_offset);
        cbuf->read_offset = (cbuf->read_offset + 1) % cbuf->capacity;
    }
    
    cbuf->size -= i;

    return i;
}

size_t CBUFFreeSpace(cbuf_t *cbuf)
{   
    return cbuf->capacity - cbuf->size;
}

cbuf_t *CBUFCreate(size_t nbytes)
{
    cbuf_t *new_cbuf = (cbuf_t *)malloc(sizeof(cbuf_t));
    if(NULL == new_cbuf)
    {
        return NULL;
    }

    new_cbuf->capacity = nbytes;
    new_cbuf->size = 0;
    new_cbuf->read_offset = 0;
    new_cbuf->buff = malloc(new_cbuf->capacity);
    if(NULL == new_cbuf->buff)
    {
        return NULL;
    }
    
    return new_cbuf;
}

size_t CBUFCapacity(cbuf_t *cbuf)
{
    return cbuf->capacity;
}

void CBUFDestroy(cbuf_t *cbuf)
{
    free(cbuf->buff);
    free(cbuf);
}

static char *GetWritePtr(cbuf_t *cbuf, int i)
{
    return (char *)cbuf->buff + 
            (cbuf->read_offset + cbuf->size) + i % cbuf->capacity;  
}

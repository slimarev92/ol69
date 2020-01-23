/**********************************
* DATA STRUCTURES: DYNAMIC VECTOR *
* CODE BY:         SASHA LIMAREV  *   
* REVIEWED BY:     SANDRA HADAD   *                          
***********************************/ 

#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "dvec.h"

#define CHANGE_FACTOR (2)

static int IsLegalElementSize(size_t size_of_element);
static int IsLegalCapacity(size_t num_elements);
static int IsNotNull(const void *ptr);
static int IsLegalIndex(dvec_t *dvec, size_t index);

struct Dvec 
{
    void *array;
    size_t size_of_element;
    size_t num_of_elements;
    size_t capacity;
};

dvec_t *DVECCreate(size_t size_of_element, size_t num_elements)
{
    dvec_t *new_dvec = NULL;

    assert(IsLegalElementSize(size_of_element) || 
           IsLegalCapacity(num_elements));

    new_dvec = (dvec_t *)malloc(sizeof(dvec_t));
    if(NULL == new_dvec)
    {
        return NULL;
    }

    new_dvec->size_of_element = size_of_element;
    new_dvec->num_of_elements = 0;
    new_dvec->capacity = num_elements;
    new_dvec->array = (void *)malloc(size_of_element * num_elements);
    if(NULL == new_dvec->array)
    {
        return NULL;
    }

    return new_dvec;
}

void DVECDestroy(dvec_t *dvec)
{
    assert(IsNotNull((void *)dvec));

    free(dvec->array);
    free(dvec);
}

size_t DVECCapacity(const dvec_t *dvec) 
{
    assert(IsNotNull((void *)dvec));

    return dvec->capacity;
}

size_t DVECSize(const dvec_t *dvec)
{
    assert(IsNotNull((void *)dvec));

    return dvec->num_of_elements;
}

int DVECPushBack(dvec_t *dvec, const void *element)
{
    void *temp_ptr = NULL;
    size_t dest = 0;

    assert(IsNotNull((void *)dvec) && IsNotNull(element));

    dest = (size_t)dvec->array + 
                          dvec->num_of_elements * dvec->size_of_element;
    memcpy((void *)dest, element, dvec->size_of_element);
    ++(dvec->num_of_elements);

    if(DVECSize(dvec) >= DVECCapacity(dvec))
    {
        temp_ptr = realloc(dvec->array, 
                     dvec->capacity * CHANGE_FACTOR * dvec->size_of_element);
        if(NULL == temp_ptr)
        {
            return 1;
        }

        dvec->capacity *= CHANGE_FACTOR;
        dvec->array = temp_ptr;
    }

    return 0;
}

void *DVECGetItemAddress(dvec_t *dvec, size_t index)
{
    assert(IsNotNull((void *)dvec) && IsLegalIndex(dvec, index));

    return (void *)((size_t)dvec->array + 
                             index * dvec-> size_of_element);
}

void DVECPopBack(dvec_t *dvec)
{
    void *array_temp = NULL;

    assert(IsNotNull((void *)dvec));

    if(0 == DVECSize(dvec))
    {
        return;
    }

    --(dvec->num_of_elements);

    if(DVECSize(dvec) <=  DVECCapacity(dvec) / (CHANGE_FACTOR * CHANGE_FACTOR))
    {
        array_temp = realloc(dvec->array, 
                     (dvec->capacity / CHANGE_FACTOR) * dvec->size_of_element);
        if(NULL == array_temp)
        {
            return;
        }

        dvec->capacity /= CHANGE_FACTOR;
        dvec->array = array_temp;
    }
}

int DVECReserve(dvec_t *dvec, size_t new_capacity)
{
    void *array_temp = NULL; 

    assert(IsNotNull((void *)dvec));

    if(new_capacity <= DVECSize(dvec))
    {
        new_capacity = DVECSize(dvec) + 1;
    }

    dvec->capacity = new_capacity;
    array_temp = realloc(dvec->array, 
                         new_capacity * dvec->size_of_element);
    if(NULL == array_temp)
    {
        return 1;
    }

    dvec->array = array_temp;
    dvec->capacity = new_capacity;

    return 0;
}

static int IsLegalElementSize(size_t size_of_element)
{
    return size_of_element > 0;
}

static int IsLegalCapacity(size_t num_elements)
{
    return num_elements > 0;
}

static int IsNotNull(const void *ptr)
{
    return ptr != NULL;
}

static int IsLegalIndex(dvec_t *dvec, size_t index)
{
    return index < DVECSize(dvec);
}

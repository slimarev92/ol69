#ifndef DVEC_H
#define DVEC_H


typedef struct Dvec dvec_t;

/* DYNAMIC VECTOR */


 
/*******************************************************************************
                            FUNCTION: DVEC CREATE
DESCRIPTION:
    - Returns new DVEC struct with a given size of element and itinial size.
PARAMETERS: 
    - Size of each element in bytes.
    - Initial capacity (in elements).
RETURN VALUE: 
    - Returns pointer to new DVEC struct if successful, NULL otherwise.
*******************************************************************************/
dvec_t *DVECCreate(size_t size_of_element,size_t num_elements);

/*******************************************************************************
                            FUNCTION: DVEC DESTROY
DESCRIPTION:
    - Frees all memory used by a DVEC struct.
PARAMETERS: 
    - Pointer to DVEC struct the user wants to destroy.
RETURN VALUE: 
    - None.
*******************************************************************************/
void DVECDestroy(dvec_t *dvec);

/*******************************************************************************
                          FUNCTION: GET ITEM ADDRESS
DESCRIPTION:
    - Returns pointer to an element inside a DVEC at a certain index.
PARAMETERS: 
    - DVEC struct.
    - Index of element to return.
RETURN VALUE: 
    - Returns pointer to an element inside the DVEC, doesn't check if index is 
    legal.
*******************************************************************************/
void *DVECGetItemAddress(dvec_t *dvec, size_t index);

/* returns 1 on allocation failure */

/*******************************************************************************
                          FUNCTION: GET PUSH BACK
DESCRIPTION:
    - Copies a given element to the end of a DVEC.
PARAMETERS: 
    - DVEC struct.
    - Pointer to element the user wants to copy.
RETURN VALUE: 
    - Returns 0 if the element is copied to DVEC, 1 if the element is copied and
    the DVEC is full and couldn't be resized. 
*******************************************************************************/
int DVECPushBack(dvec_t *dvec, const void *element);

/*******************************************************************************
                          FUNCTION: GET POP BACK
DESCRIPTION:
    - Removes last element from a DVEC. Resizes the DVEC if neccesary.
PARAMETERS: 
    - DVEC struct.
RETURN VALUE: 
    - None.
*******************************************************************************/
void DVECPopBack(dvec_t *dvec);

/*******************************************************************************
                          FUNCTION: SIZE
DESCRIPTION:
    - Return current amount of elements in a DVEC.
PARAMETERS: 
    - DVEC struct.
RETURN VALUE: 
    - Number of elements in the DVEC.
*******************************************************************************/
size_t DVECSize(const dvec_t *dvec);

/*******************************************************************************
                          FUNCTION: CAPACITY
DESCRIPTION:
    - Return current capacity (in elements) of a DVEC.
PARAMETERS: 
    - DVEC struct.
RETURN VALUE: 
    - Capacity of the DVEC.
*******************************************************************************/
size_t DVECCapacity(const dvec_t *dvec);

/*******************************************************************************
                          FUNCTION: RESERVE
DESCRIPTION:
    - Allow the user to "reserve" a minimum capacity. If the requested capacity 
    is larger then current size then DVEC is expanded, if it's smaller than 
    current size the DVEC is expanded to current size plus one.  
PARAMETERS: 
    - DVEC struct.
    - Capacity the user wishes to reserve. 
RETURN VALUE: 
    - 0 if everything is successful, 1 if the DVEC resizing failed.
*******************************************************************************/
int DVECReserve(dvec_t *dvec, size_t new_capacity);

#endif

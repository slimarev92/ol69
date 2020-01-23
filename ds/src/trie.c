/********************************
* DS:          TRIE             *
* CODE BY:     SASHA LIMAREV    *   
* REVIEWED BY: ROI KRAMER       *                          
********************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <math.h> /* pow */
#include <string.h> /* memcpy */

#include "trie.h"

#define BYTE (8)
#define MASK_START (((uchar_t)0x1) << (BYTE - 1))
#define N_BYTES(n_bits) ((n_bits) / BYTE)

typedef unsigned char uchar_t;

typedef struct node
{
    int is_full;
    struct node *children[2];
} node_t;

struct trie
{
    size_t n_bits;
    node_t *root;
};

static node_t *CreateNode();
static void DestroyNode(node_t *);
static int HasChild(node_t *, uchar_t);
enum trie_status InsertRec(node_t *, const uchar_t *,uchar_t, size_t);
static void UpdateFullness(node_t *);
static size_t CountRec(node_t *, size_t);
static enum trie_status RemoveRec(node_t *, const uchar_t *, uchar_t, size_t);
enum trie_status InsertRecClose(node_t *, uchar_t *,uchar_t, size_t, uchar_t *);
static void FlipBytes(void *, size_t);
static int IslittleEndian();
static uchar_t *GetAdjustedData(trie_t *, const void *);
static enum trie_status CreateMissingNode(node_t *node, uchar_t child);
static void CopyBit(uchar_t *, uchar_t, uchar_t);
static uchar_t UpdateMask(const uchar_t mask);
static uchar_t *UpdateByte(const uchar_t *, uchar_t);
static uchar_t CalculateChild(const uchar_t *, const uchar_t);

trie_t *TrieCreate(size_t n_bits)
{
    trie_t *new_trie = NULL;
    node_t *root = NULL;

    assert(0 < n_bits);

    new_trie = (trie_t *)malloc(sizeof(trie_t));
    if(NULL == new_trie)
    {
        return NULL;
    }

    root = CreateNode();
    if(NULL == root)
    {
        free(new_trie);
        new_trie = NULL;
        return NULL;
    }

    new_trie->root = root;
    new_trie->n_bits = n_bits;

    return new_trie;
}

enum trie_status TrieRemove(trie_t *trie, const void *value)
{
    uchar_t *data_adjusted = NULL;
    enum trie_status status = REMOVED;   

    assert(trie);
    assert(value);

    data_adjusted = GetAdjustedData(trie, value);
    if(NULL == data_adjusted)
    {
        return MEM_FAIL;
    }

    status = RemoveRec(trie->root, data_adjusted, MASK_START, trie->n_bits);

    free(data_adjusted);

    return status;
}
   
/* recursively remove all the nodes */
static enum trie_status RemoveRec(node_t *node, const uchar_t *byte,
                                                    uchar_t mask, size_t n_bits)
{
    unsigned int child = 0;
    enum trie_status ret = 0;

    if(NULL == node || (0 == n_bits && !node->is_full))
    {
        return NREMOVED;
    }    
    else if(0 == n_bits && node->is_full)
    {
        node->is_full = 0;
        return REMOVED;
    }
    else
    {
        child = CalculateChild(byte, mask);
        mask = UpdateMask(mask);
        byte = UpdateByte(byte, mask);
        ret = RemoveRec(node->children[child], byte, mask, --n_bits);

        UpdateFullness(node);

        return ret;
    }
}

void TrieDestroy(trie_t *trie)
{
    assert(trie);

    DestroyNode(trie->root);
    trie->root = NULL;

    free(trie);
    trie = NULL;
}

/* recursively destroy trie node and all of its children*/
static void DestroyNode(node_t *node)
{
    if(NULL == node)
    {
        return;
    }
    else
    {
        DestroyNode(node->children[0]);
        DestroyNode(node->children[1]);

        free(node);
        node = NULL;
    }
}

enum trie_status TrieInsertExact(trie_t *trie, const void *value)
{
    enum trie_status status = ADDED;
    uchar_t *data_adjusted = NULL;

    assert(trie);
    assert(value);
    
    if(trie->root->is_full)
    {
        return FULL;
    }

    data_adjusted = GetAdjustedData(trie, value);
    if(NULL == data_adjusted)
    {
        return MEM_FAIL;
    }

    status = InsertRec(trie->root, data_adjusted, MASK_START, trie->n_bits);

    free(data_adjusted);

    return status;
}

/* copies the users data to the heap, flips all the bytes if little endian */
static uchar_t *GetAdjustedData(trie_t *trie, const void *value)
{
    uchar_t *data_adjusted = (uchar_t *)malloc(N_BYTES(trie->n_bits));
    if(NULL == data_adjusted)
    {
        return NULL;
    }

    memcpy(data_adjusted, value, N_BYTES(trie->n_bits));

    if(IslittleEndian())
    {
        FlipBytes((void *)data_adjusted, N_BYTES(trie->n_bits));
    }    

    return data_adjusted;
}

enum trie_status TrieInsertClose(trie_t *trie, const void *value, void *res)
{
    enum trie_status status = ADDED;
    uchar_t *data_adjusted = NULL;
    
    assert(trie);
    assert(value);

    if(trie->root->is_full)
    {
        return FULL;
    }

    data_adjusted = GetAdjustedData(trie, value);
    if(NULL == data_adjusted)
    {
        return MEM_FAIL;
    }

    status = InsertRecClose(trie->root,data_adjusted, 
                                                 MASK_START, trie->n_bits, res);
    if(ADDED == status)
    {
        if(IslittleEndian())
        {
             FlipBytes(res, N_BYTES(trie->n_bits));
        }

        status = (memcmp(value, res, N_BYTES(trie->n_bits)) == 0) ? 
                                                            ADDED : ADDED_EXIST;
    }

    free(data_adjusted);
    return status;
}

size_t TrieCount(trie_t *trie)
{
    assert(trie);

    return CountRec(trie->root->children[0], trie->n_bits - 1) + 
           CountRec(trie->root->children[1], trie->n_bits - 1);
}

/* recursively counts the number of nodes in a trie */
static size_t CountRec(node_t *node, size_t n_bits)
{
    if(NULL == node) /* +0 */
    {
        return 0;
    }
    else if(node->is_full) /* + 2^n_bits */
    {
        return (size_t)pow(2, n_bits);
    }
    else
    {
        return CountRec(node->children[0], n_bits - 1) + 
               CountRec(node->children[1], n_bits - 1);
    }
}

enum trie_status InsertRecClose(node_t *node, uchar_t *byte,
                                      uchar_t mask, size_t n_bits, uchar_t *res)
{
    unsigned int child = 0; 

    assert(node);
    assert(byte);
    assert(res);

    if(0 == n_bits)
    {
        node->is_full = 1;
        return ADDED;
    }
    else
    {
        child = CalculateChild(byte, mask);

        /* if value exists in tree already, create node in nearest branch */
        if(NULL != node->children[child] && node->children[child]->is_full)
        {     
            child = !child;
        }
                    
        if(MEM_FAIL == CreateMissingNode(node, child))
        {
            return MEM_FAIL;
        }

        CopyBit(res, child, mask);

        res =  (0x1 == mask) ? res + 1 : res;
        mask = UpdateMask(mask);
        byte =  UpdateByte(byte, mask);
        InsertRecClose(node->children[child], byte, mask, --n_bits, res);

        UpdateFullness(node);

        return ADDED;
    }
}

enum trie_status InsertRec(node_t *node, const uchar_t *byte,
                                                    uchar_t mask, size_t n_bits)
{
    unsigned int child = 0;
    int ret = 0;

    assert(node);
    assert(byte);

    if(0 == n_bits && !node->is_full)
    {
        node->is_full = 1;
        return ADDED;
    }
    else if(node->is_full)
    {
        return NADDED_EXIST;
    }
    else
    {
        child = CalculateChild(byte, mask);

        if(MEM_FAIL == CreateMissingNode(node, child))
        {
            return MEM_FAIL;
        }

        mask = UpdateMask(mask);
        ret = InsertRec(node->children[child], UpdateByte(byte, mask),
                                                                mask, --n_bits);

        UpdateFullness(node);

        return ret;
    }
}

/* calculates child to move to */
static uchar_t CalculateChild(const uchar_t *byte, const uchar_t mask)
{
    return (0 == (*byte & mask)) ? 0 : 1;
}

/* updates node's is_full member based on its children */
static void UpdateFullness(node_t *node)
{
    uchar_t is_full0 = HasChild(node, 0) && node->children[0]->is_full;
    uchar_t is_full1 = HasChild(node, 1) && node->children[1]->is_full;

    node->is_full = is_full0 && is_full1;
}

/* creates blank node, return pointer to the new node */
static node_t *CreateNode()
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    if(NULL == node)
    {
        return NULL;
    }

    node->is_full = 0;
    node->children[0] = NULL;
    node->children[1] = NULL;  

    return node;
}

/* returns one if the system is based on little endian ordering */
static int IslittleEndian()
{
    const unsigned short num = 0x1234;
    const uchar_t byte = 0x34;

    return *(uchar_t *)&num == byte;
}

/* returns next byte if necessary */
static uchar_t *UpdateByte(const uchar_t *byte, uchar_t mask)
{
    return (MASK_START == mask) ? (uchar_t *)byte + 1 :  (uchar_t *)byte;
}

/* either shifts mask by one bit right or resets it to starting value */
static uchar_t UpdateMask(const uchar_t mask)
{
    return (0x1 == mask) ? MASK_START : mask >> 1;
}

/* copy next bit to result */
static void CopyBit(uchar_t *res, uchar_t child, uchar_t mask)
{
    mask = (0 == child) ? 0 : mask;

    *res = *res | mask;
}

/* create new node if necessary */
static enum trie_status CreateMissingNode(node_t *node, uchar_t child)
{
    if(NULL == node->children[child]) 
    { 
        node->children[child] = CreateNode();
        return NULL ==  node->children[child] ? MEM_FAIL : ADDED;
    }
    else
    {
        return NADDED_EXIST;
    }
}

/* flips byte array */
static void FlipBytes(void *base, size_t n_bytes)
{
    uchar_t *left = (uchar_t *)base;
    uchar_t *right = (uchar_t *)base + (n_bytes - 1);
    uchar_t temp = 0;

    while(left < right)
    {
        temp = *left;
        *left = *right;
        *right = temp;

        ++left;
        --right;
    }
}

/* return 1 if node has child 1/0, 0 otherwise */
static int HasChild(node_t *node, uchar_t child)
{
    return NULL != node->children[child];
}


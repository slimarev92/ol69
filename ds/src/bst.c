/***************************************
* DATA STRUCTURES: BINARY SEARCH TREE  *
* CODE BY:         SASHA LIMAREV       *   
* REVIEWED BY:     DANIEL MAIZEL       *                          
****************************************/ 
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "../include/bst.h" /* BST header file */           

enum side {LEFT, RIGHT};

typedef struct bst_node
{
    void *data;
    bst_it_t parent;
    bst_it_t children[2];
} bst_node_t;

struct bst
{
    struct bst_node dummy;
    cmp_f compare;
    const void *params;
};

static void SetDummyChildren(bst_node_t *node, bst_t *bst);
static bst_it_t CreateNode(void *data, bst_t *bst);
static int IsDummy(bst_node_t *node);
static bst_node_t *GetDummy(bst_t *bst);
static int HasRChild(bst_node_t *node);
static int HasLChild(bst_node_t *node);
static bst_node_t *GetLChild(bst_node_t *node);
static bst_node_t *GetRChild(bst_node_t *node);
static enum side DetermineLastSide(bst_node_t *curr, bst_node_t *last);
static int IsLeaf(bst_node_t *node);
static bst_node_t *GetChild(bst_node_t *node, enum side child_side);
static enum side OppositeSide(enum side side);
static int HasChild(bst_node_t *node, enum side child_side);
static bst_it_t BSTInOrder(bst_it_t node, enum side side);

bst_t *BSTCreate(cmp_f compare, const void *params)
{
    bst_t *new_bst = NULL;
    
    assert(NULL != compare);

    new_bst = (bst_t *)malloc(sizeof(bst_t));
    if(NULL == new_bst)
    {
        return NULL;
    }

    new_bst->params = params;
    new_bst->compare = compare;
    new_bst->dummy.data = (void *)0xDEADBEEF;
    new_bst->dummy.parent = NULL;
    SetDummyChildren(GetDummy(new_bst), new_bst);

    return new_bst;
}

void BSTDestroy(bst_t *bst)
{
    bst_node_t *curr = NULL;
    bst_node_t *last = NULL;

    assert(NULL != bst);

    curr = GetLChild(GetDummy(bst));

    while(!IsDummy(curr))
    {
        if(HasLChild(curr))
        {
            curr = GetLChild(curr);
        }
        else if(HasRChild(curr))
        {
            curr = GetRChild(curr);
        }
        else
        {
            last = curr;
            curr = curr->parent;
            curr->children[DetermineLastSide(curr, last)] = GetDummy(bst);
            free(last);
        }
    }

    free(bst);
}

bst_it_t BSTPrev(bst_it_t node)
{
    return BSTInOrder(node, LEFT);
}

bst_it_t BSTNext(bst_it_t node)
{

    return BSTInOrder(node, RIGHT);
}

static bst_it_t BSTInOrder(bst_it_t node, enum side side)
{
    bst_node_t *curr = NULL;
    enum side opposite_side = OppositeSide(side);
    
    assert(NULL != node);

    curr = node;

    if(HasChild(node, side))
    {   
        curr = GetChild(curr, side);

        while(HasChild(curr, opposite_side))
        {   
            curr = GetChild(curr, opposite_side);
        }

        return curr;
    }

    while(GetChild(curr->parent, opposite_side) != curr)
    {
        curr = curr->parent;
    }

    return curr->parent;
}


bst_it_t BSTEnd(const bst_t *bst)
{
    return GetDummy((bst_t *)bst);
}

bst_it_t BSTBegin(const bst_t *bst)
{
    bst_node_t *curr = NULL;
    
    assert(NULL != bst);

    curr =  GetDummy((bst_t *)bst);

    while(HasLChild(curr))
    {
        curr = GetLChild(curr);
    }

    return curr;
}

bst_it_t BSTInsert(bst_t *bst, void *data)
{
    bst_node_t *new_node = NULL;
    bst_node_t *curr = NULL;
    int curr_compare = 0;
    bst_node_t *curr_parent = NULL;
    bst_node_t *dummy = GetDummy(bst);
    enum side curr_direction = LEFT;

    assert(NULL != bst);

    new_node = CreateNode(data, bst);
    if(NULL == new_node)
    {
        return NULL;
    }

    curr_parent = dummy;
    curr = GetLChild(dummy);

    while(!IsDummy(curr))
    {   
       curr_compare = bst->compare(BSTGetData(curr), data, bst->params);    

       if(0 == curr_compare)
       {
            free(new_node);
            return dummy; 
       }
       else if(0 < curr_compare)
       {
            curr_parent = curr;
            curr = GetLChild(curr);
            curr_direction = LEFT;
       }      
       else
       {
            curr_parent = curr;          
            curr = GetRChild(curr);
            curr_direction = RIGHT;
       }
    }
    
    curr_parent->children[curr_direction] = new_node;
    new_node->parent = curr_parent;

    return new_node;
}

void BSTRemove(bst_it_t node)
{
    bst_node_t *curr = NULL;
    bst_node_t *next = NULL;
    int was_free = 0;

    assert(NULL != node);

    curr = node;

    while(!was_free)
    {
        if(IsLeaf(curr))
        {
            curr->parent->children[DetermineLastSide(curr->parent, curr)] =
                                                                GetLChild(curr);
            free(curr);
            was_free = 1;
        }
        else
        {
            next = BSTNext(curr);
            curr->data = next->data;
            curr = next;
        }
    }
}

size_t BSTCount(const bst_t *bst)
{
    size_t counter = 0;
    bst_node_t *curr = NULL;

    assert(NULL != bst);

    curr = BSTBegin(bst);

    while(!IsDummy(curr))
    {
        ++counter;
        curr = BSTNext(curr);
    }

    return counter;
}

int BSTForEach(bst_it_t from, bst_it_t to, act_f action, const void *args)
{
    int curr_status = 0;

    assert(NULL != from && NULL != to && NULL != action);
    
    while(from != to)
    {
        curr_status = action(BSTGetData(from), args);
        
        if(0 != curr_status)
        {
            break;
        }

        from = BSTNext(from);
    }

    return curr_status;
}

bst_it_t BSTFind(const bst_t *bst, const void *data)
{
    bst_node_t *curr = NULL;
    int curr_compare = 0;

    assert(NULL != bst);

    curr = GetLChild(GetDummy((bst_t *)bst));

    while(!IsDummy(curr))
    {   
       curr_compare = bst->compare(BSTGetData(curr), data, bst->params);    

       if(0 == curr_compare)
       {
            return curr; 
       }
       else if(0 < curr_compare)
       {
            curr = GetLChild(curr);
       }      
       else
       {       
            curr = GetRChild(curr);
       }
    }
           
    return NULL;
}

static enum side DetermineLastSide(bst_node_t *curr, bst_node_t *last)
{
    return curr->children[LEFT] == last ? LEFT : RIGHT;
}

int BSTIsEmpty(const bst_t *bst)
{
    return GetDummy((bst_t *)bst) == GetDummy((bst_t *)bst)->children[LEFT];
}

void *BSTGetData(bst_it_t node)
{
    return node->data;
}

static int HasChild(bst_node_t *node, enum side child_side)
{
    return !IsDummy(GetChild(node, child_side));
}

static int HasRChild(bst_node_t *node)
{
    return !IsDummy(GetRChild(node));
}

static int HasLChild(bst_node_t *node)
{
    return !IsDummy(GetLChild(node));
}

static bst_it_t CreateNode(void *data, bst_t *bst)
{
    bst_node_t *new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
    if(NULL == new_node)
    {
        return NULL;
    }

    new_node->data = data;
    new_node->parent = NULL;
    SetDummyChildren(new_node, bst);

    return new_node;
}

static bst_node_t *GetChild(bst_node_t *node, enum side child_side)
{
    return node->children[child_side];
}

static bst_node_t *GetLChild(bst_node_t *node)
{
    return node->children[LEFT];
}

static bst_node_t *GetRChild(bst_node_t *node)
{
    return node->children[RIGHT];
}

static int IsLeaf(bst_node_t *node)
{
    return !HasLChild(node) && !HasRChild(node);
}

static bst_node_t *GetDummy(bst_t *bst)
{
    return &bst->dummy;
}

static int IsDummy(bst_node_t *node)
{
    return node->children[RIGHT] == node;
}

static void SetDummyChildren(bst_node_t *node, bst_t *bst)
{
    node->children[LEFT] = GetDummy(bst);
    node->children[RIGHT] = GetDummy(bst);
}

static enum side OppositeSide(enum side side)
{
    return LEFT == side ? RIGHT : LEFT;
}

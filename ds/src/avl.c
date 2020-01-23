/***************************************
* DATA STRUCTURES: BALANCED BST (AVL)  *
* CODE BY:         SASHA LIMAREV       *   
* REVIEWED BY:     ALEX ZILBERMAN      *                          
****************************************/ 
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <sys/types.h> /* ssize_t */

#include "avl.h" /* AVL header file */           

enum side {LEFT, RIGHT};

typedef struct avl_node
{
    void *data;
    struct avl_node *children[2];
    ssize_t height;
} avl_node_t;

 struct avl
{
    avl_node_t *root;
    avl_cmp_f compare;
    const void *params;
};

static void DestroyRec(avl_node_t *node);
static avl_node_t *InsertRec(avl_node_t *node, void *data, avl_cmp_f cmp, 
                                                    void *param,  int *status);
static void *DiveLeft(avl_node_t *node);
static avl_node_t *CreateNode(void *data);
static void UpdateHeight(avl_node_t *node);
static int HasChild(const avl_node_t *node, enum side side);
static size_t GetSubtreeHeight(avl_node_t *node, enum side side);
static avl_node_t *BalanceTree(avl_node_t *node);
static avl_node_t *BalanceSimple(avl_node_t *node, enum side heavy_unbalanced);
static avl_node_t *BalanceComplex(avl_node_t *node, enum side heavy_unbalanced);
static ssize_t GetBalanceFactor(avl_node_t *node);
enum side GetHeavySide(avl_node_t *node);
static avl_node_t *RemoveRec(avl_t *avl, avl_node_t *node, void *data);
static int ForEachRec(avl_node_t *node, avl_act_f act, const void *args);
static avl_node_t *FindRec(avl_node_t *node, const void *key,
                                                      avl_cmp_f cmp, void *arg);
static size_t GetSizeRec(avl_node_t *node);

avl_t *AVLCreate(avl_cmp_f compare, const void *params)
{
    avl_t *new_avl = NULL;

    assert(NULL != compare);

    new_avl = (avl_t *)malloc(sizeof(avl_t));
    if(NULL == new_avl)
    {
        return NULL;
    }

    new_avl->root = NULL;
    new_avl->compare = compare;
    new_avl->params = params;

    return new_avl;
}

void AVLDestroy(avl_t *avl)
{
    DestroyRec(avl->root);
    free(avl);
}

static void DestroyRec(avl_node_t *node)
{
    if(NULL != node)
    {
        DestroyRec(node->children[RIGHT]);
        DestroyRec(node->children[LEFT]);

        free(node);
    }
}

int AVLInsert(avl_t *avl, void *data)
{
    int status = 0;

    assert(NULL != avl);
    assert(NULL != data);

    avl->root = InsertRec(avl->root, data, avl->compare,
                                                  (void *)avl->params, &status);

    return status;
}

static avl_node_t *InsertRec(avl_node_t *node, void *data, avl_cmp_f cmp, 
                                                      void *param,  int *status)
{
    ssize_t cmp_result = 0;
    enum side move_to = LEFT;
    avl_node_t *new_node = NULL;

    assert(NULL != data);

    if(NULL == node)
    {
        new_node =  CreateNode(data);
        *status = NULL == new_node ? 1 : 0;
        return new_node;
    }

    cmp_result = cmp(data, node->data, param);

    if(0 == cmp_result)
    {
        *status = 1;
    }
    else
    {
        move_to = (cmp_result > 0) ? RIGHT : LEFT;

        node->children[move_to] = 
                   InsertRec(node->children[move_to], data, cmp, param, status);
    }   
    
    UpdateHeight(node);

    return BalanceTree(node);
}

static avl_node_t *CreateNode(void *data)
{
    avl_node_t *new_node = (avl_node_t *)malloc(sizeof(avl_node_t));
    if(NULL == new_node)
    {
        return NULL;
    }

    new_node->data = data;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;
    new_node->height = 0;

    return new_node;
}

static void UpdateHeight(avl_node_t *node)
{
    ssize_t l_height =  GetSubtreeHeight(node, LEFT);
    ssize_t r_height =  GetSubtreeHeight(node, RIGHT);

    size_t max_height = (l_height > r_height) ? l_height : r_height;
    
    node->height = max_height + 1;
}

static size_t GetSubtreeHeight(avl_node_t *node, enum side side)
{
    return (!HasChild(node, side)) ? -1 : node->children[side]->height;
}

static int HasChild(const avl_node_t *node, enum side side)
{
    return NULL != node->children[side];
}

static avl_node_t *BalanceTree(avl_node_t *node)
{
    ssize_t bal_fact = GetBalanceFactor(node);
    enum side heavy_balanced = LEFT;
    enum side heavy_unbalanced = RIGHT;

    if(2 <= bal_fact || -2 >= bal_fact)
    {
        heavy_unbalanced = GetHeavySide(node);
        heavy_balanced = GetHeavySide(node->children[heavy_unbalanced]);

        if(heavy_balanced == heavy_unbalanced)
        {
            return BalanceSimple(node, heavy_unbalanced);
        }
        else
        {
            return BalanceComplex(node, heavy_unbalanced);
        }
    }
    else
    {
        return node;
    }
}

static ssize_t GetBalanceFactor(avl_node_t *node)
{
    ssize_t l_height = GetSubtreeHeight(node, LEFT);
    ssize_t r_height = GetSubtreeHeight(node, RIGHT);

    return r_height - l_height;
}

enum side GetHeavySide(avl_node_t *node)
{
    ssize_t l_height =  GetSubtreeHeight(node, LEFT);
    ssize_t r_height =  GetSubtreeHeight(node, RIGHT);

    return l_height >= r_height ? LEFT : RIGHT;
}

static avl_node_t *BalanceSimple(avl_node_t *node, enum side heavy_unbalanced)
{
    avl_node_t *new_child = NULL;
    avl_node_t *new_parent = NULL;
    enum side light_unbalanced = (LEFT == heavy_unbalanced) ? RIGHT : LEFT;
    
    /* get a new parent + child for the unbalanced node */
    new_parent = node->children[heavy_unbalanced];
    new_child = new_parent->children[light_unbalanced];

    /* connect parent and child */
    new_parent->children[light_unbalanced] = node;
    node->children[heavy_unbalanced] = new_child;

    /* update height */
    UpdateHeight(new_parent->children[LEFT]);
    UpdateHeight(new_parent->children[RIGHT]);
    UpdateHeight(new_parent);

    return new_parent;
}

static avl_node_t *BalanceComplex(avl_node_t *node, enum side heavy_unbalanced)
{
    avl_node_t *unbalanced_child = NULL;
    avl_node_t *balanced_child = NULL;
    avl_node_t *unbalanced_heavy_child = NULL;
    avl_node_t *new_parent = NULL;
    enum side heavy_balanced = (LEFT == heavy_unbalanced) ? RIGHT : LEFT;
    
    /* get new parent and its children (the "heavy" child of the unablanced node's heavy child is the new parent) */
    new_parent = node->children[heavy_unbalanced]->children[heavy_balanced];
    unbalanced_child = new_parent->children[heavy_balanced];
    balanced_child = new_parent->children[heavy_unbalanced];

    /* set unbalanced node and its "heavy" child as the children */
    new_parent->children[heavy_balanced] = node;
    new_parent->children[heavy_unbalanced] = node->children[heavy_unbalanced];

    /* replace missing children of the unbalanced node and its heavy child */
    unbalanced_heavy_child =  node->children[heavy_unbalanced];
    node->children[heavy_unbalanced] = unbalanced_child;
    unbalanced_heavy_child->children[heavy_balanced] = balanced_child;

    /* update height */
    UpdateHeight(new_parent->children[LEFT]);
    UpdateHeight(new_parent->children[RIGHT]);
    UpdateHeight(new_parent);
    
    return new_parent;
}

void AVLRemove(avl_t *avl, const void *data)
{
    assert(NULL != data); /* is this needed? */
    assert(NULL != avl);

    avl->root = RemoveRec(avl, avl->root, (void *)data);
}

static avl_node_t *RemoveRec(avl_t *avl, avl_node_t *node, void *data)
{
    ssize_t cmp_res = avl->compare(data, node->data, avl->params);
    enum side move_to = LEFT;
    avl_node_t *ret_val = node;
    int has_left = 0;
    int has_right = 0;

    if(0 != cmp_res)
    {
        move_to = (cmp_res > 0) ? RIGHT : LEFT;
        node->children[move_to] = RemoveRec(avl, node->children[move_to], data);
        UpdateHeight(node);

        return BalanceTree(node); 
    }
    else
    {
        has_left = HasChild(node, LEFT);
        has_right = HasChild(node, RIGHT);

        if(!has_left && !has_right) 
        {
            free(node);
            return NULL;
        }
        else if(has_left && has_right)
        {
            node->data = DiveLeft(node->children[RIGHT]);
            node->children[RIGHT] = RemoveRec(avl, node->children[RIGHT], 
                                                                    node->data);
            UpdateHeight(node);

            return BalanceTree(node); 
        }
        else
        {
            ret_val = has_left ? node->children[LEFT] : node->children[RIGHT]; 
            free(node);
            return ret_val;
        }
    }
}

static void *DiveLeft(avl_node_t *node)
{
    return HasChild(node, LEFT) ? DiveLeft(node->children[LEFT]) : node->data;
}

int AVLForEach(avl_t *avl, avl_act_f act, const void *args)
{
    return ForEachRec(avl->root, act, args);
}

static int ForEachRec(avl_node_t *node, avl_act_f act, const void *args)
{
    size_t ret_status = 0;

    if(HasChild(node, LEFT))
    {
        ret_status = ForEachRec(node->children[LEFT], act, args);
    }

    /* if left side was completed with no errors, continue */
    if(0 == ret_status)
    {
        ret_status = act(node->data, args);
    }

    /* if left side & parent of right finished with no errors, continue */
    if(0 == ret_status && HasChild(node, RIGHT))
    {
        ret_status = ForEachRec(node->children[RIGHT], act, args);
    }

    return ret_status;
}

void *AVLFind(const avl_t *avl, const void *key)
{
    avl_node_t *target = NULL;   

    assert(NULL != avl);

    target = FindRec(avl->root, key, avl->compare, (void *)avl->params);

    return NULL == target ? NULL : target->data;
}

static avl_node_t *FindRec(avl_node_t *node, const void *key,
                                                       avl_cmp_f cmp, void *arg)
{
    ssize_t cmp_result = 0;
    enum side move_to = LEFT;

    if(NULL != node)
    {
        cmp_result = cmp(key, node->data, arg);
        move_to = (cmp_result > 0) ? RIGHT : LEFT;

        if(0 == cmp_result)
        {
            return node;
        }
        else
        {
            return FindRec(node->children[move_to], key, cmp, arg);
        }
    }

    return NULL;
}

size_t AVLGetSize(const avl_t *avl)
{
    return AVLIsEmpty(avl) ? 0 : GetSizeRec(avl->root);
}

static size_t GetSizeRec(avl_node_t *node)
{
    return (NULL == node) ? 0 : GetSizeRec(node->children[LEFT]) +
                                          GetSizeRec(node->children[RIGHT]) + 1; 
}

int AVLIsEmpty(const avl_t *avl)
{
    return (NULL == avl->root) ? 1 : 0;
}

size_t AVLGetHeight(const avl_t *avl)
{
    return avl->root->height;
}
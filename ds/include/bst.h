#ifndef BST_H
#define BST_H

#ifndef CMP_F
#define CMP_F
typedef int (*cmp_f)(const void *data1, const void *data2, const void *params);
#endif

#ifndef ACT_F 
#define ACT_F
typedef int (*act_f)(void *data, const void *args);
#endif

typedef struct bst_node *bst_it_t;
typedef struct bst bst_t;

/* returns a BST */
bst_t *BSTCreate(cmp_f compare, const void *params); 

/* destroy bst */
void BSTDestroy(bst_t *bst);

/* return NULL if space is full/failed */
bst_it_t BSTInsert(bst_t *bst, void *data);

/* removes the iterator */
void BSTRemove(bst_it_t node);

/* returns NULL if not found */
bst_it_t BSTFind(const bst_t *bst, const void *data);

/* 0 succes, 1 failure */
int BSTForEach(bst_it_t from, bst_it_t to, act_f action, const void *args);

/* return NULL if  there is no Prev */
bst_it_t BSTPrev(bst_it_t node);

/* return NULL if  there is no Next */
bst_it_t BSTNext(bst_it_t node);

/* Gets Data */
void *BSTGetData(bst_it_t node);

/* How Many Elements */
size_t BSTCount(const bst_t *bst);

/* 0 succes, 1 failure */
int BSTIsEmpty(const bst_t *bst);

/* return iterator to the begin */
bst_it_t BSTBegin(const bst_t *bst);

/* return iterator to the end */
bst_it_t BSTEnd(const bst_t *bst);

#endif


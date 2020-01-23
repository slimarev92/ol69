/*****************
Recursive implementation of balanced avl binary search tree
*****************/

#ifndef AVL_H
#define AVL_H

#include <stddef.h> /* size_t */

/* Return negative if data1 < data2.
	return zero if data1 = data2,
	return positive if  data1 > data2*/
typedef int (*avl_cmp_f)(const void *data1, const void *data2, const void *params);
typedef int (*avl_act_f)(void *data, const void *args);

/***************************
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
****************************/

typedef struct avl avl_t;

/* mustnt pass NULL pointer*/
avl_t *AVLCreate(avl_cmp_f Compare, const void *params);

/* implement post order. Mustnt pass NULL pointer*/
void AVLDestroy(avl_t *avl);

/* remember to add balancing. return status */
int AVLInsert(avl_t *avl, void *data);

/* (Remember to add balancing.). Removing non existent data - does nothing */
void AVLRemove(avl_t *avl, const void *key);

/* (Implement in order.) Return last action status. Abort action if failed. Mustn't pass empty tree. */
int AVLForEach(avl_t *avl, avl_act_f Act, const void *args);

/* Return pointer to user data. If none found return NULL. Mustn't pass empty tree. */
void *AVLFind(const avl_t *avl, const void *key);

/*Implement pre order */
size_t AVLGetSize(const avl_t *avl);

int AVLIsEmpty(const avl_t *avl);

/* a height of a leaf is 0. Mustn't pass empty tree. */
size_t AVLGetHeight(const avl_t *avl);

#endif

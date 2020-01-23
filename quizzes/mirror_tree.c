/* 22.08.19 */
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#define TREE_SIZE (6)

enum side {LEFT, RIGHT};

typedef struct bin_tree
{
	int val;
	struct bin_tree *left_child;
	struct bin_tree *right_child;
} bin_tree_t;

void MirrorTree(bin_tree_t *tree)
{
	bin_tree_t *temp = NULL;

	if(NULL == tree)
	{
		return;
	}

	MirrorTree(tree->left_child);
	MirrorTree(tree->right_child);

	temp = tree->left_child;
	tree->left_child = tree->right_child;
	tree->right_child = temp;
}

void PrintTreeInOrder(bin_tree_t *tree)
{
	if(NULL == tree)
	{
		return;
	}

	PrintTreeInOrder(tree->left_child);
	printf("%d\n", tree->val);
	PrintTreeInOrder(tree->right_child);
}

bin_tree_t *CreateNode(int val)
{
	bin_tree_t *node = malloc(sizeof(*node));
	if(NULL == node)
	{
		return NULL;
	}

	node->val = val;
	node->left_child = NULL;
	node->right_child = NULL;

	return node;
}

bin_tree_t *AddToTree(bin_tree_t *tree, int side, int val)
{
	if(LEFT == side)
	{
		tree->left_child = CreateNode(val);
		if(NULL == tree->left_child)
		{
			return NULL;
		}

		return tree->left_child;
	}
	else	
	{
		tree->right_child = CreateNode(val);
		if(NULL == tree->right_child)
		{
			return NULL;
		}

		return tree->right_child;;
	}
}

void DestroyTree(bin_tree_t *tree)
{
	if(NULL == tree)
	{
		return;
	}

	DestroyTree(tree->left_child);
	DestroyTree(tree->right_child);

	free(tree);
}

int main()
{
	bin_tree_t **tree_arr = malloc(sizeof(*tree_arr) * TREE_SIZE);

	tree_arr[0] = CreateNode(1);

	tree_arr[1] = AddToTree(tree_arr[0], LEFT, 2);
	tree_arr[2] = AddToTree(tree_arr[0], RIGHT, 3);
	
	tree_arr[3] = AddToTree(tree_arr[1], LEFT, 4);
	tree_arr[4] = AddToTree(tree_arr[1], RIGHT, 5);
	
	tree_arr[5] = AddToTree(tree_arr[2], LEFT, 6);

	PrintTreeInOrder(tree_arr[0]);
	/* expected order 4 2 5 1 6 3 */

	printf("-------------------\n");

	MirrorTree(tree_arr[0]);

	PrintTreeInOrder(tree_arr[0]);
	/* expected order 3 6 1 5 2 4 */
	
	DestroyTree(tree_arr[0]);
	free(tree_arr);

	return 0;
}
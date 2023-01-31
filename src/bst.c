/**
 * @file bst.c
 * @brief Implementation of binary search tree.
 * @author Christian Salomonsen
 */

#include "bst.h"
#include "common.h"
#include "printing.h"
#include <stdlib.h>

struct node;

/**
 * @typedef node
 * @brief Node that stores elements within trees.
 *
 */
typedef struct node node_t;

struct tree {
	node_t *root;
	size_t size;
	cmpfunc_t cmp;
};

struct node {
	void *elem;
	node_t *left;
	node_t *right;
};


/**
 * @brief Create a new node.
 *
 * @param elem 
 * @return node
 */
static node_t *newnode(void *elem)
{
	node_t *node = malloc(sizeof(node_t));

	if (!node)
		ERROR_PRINT("newnode: Malloc failed!");

	node->elem = elem;
	node->right = NULL;
	node->left =NULL;

	return node;
}


/**
 * @brief Delete a node.
 *
 * @param root 
 */
static void deletenode(node_t *root)
{
	if (!root)
		return;

	if (root->left)
		deletenode(root);

	if (root->right)
		deletenode(root);

	free(root);
}

/**
 * @brief Create a binary search tree.
 *
 * @param cmpfunc comparison function: int (*cmpfunc_t) (void*, void*)
 * @return tree
 */
tree_t *tree_create(cmpfunc_t cmpfunc)
{
	tree_t *tree = malloc(sizeof(tree_t));

	if (!tree)
		ERROR_PRINT("bst_create: Malloc failed!");

	tree->cmp = cmpfunc;
	tree->root = NULL;
	tree->size = 0;

	return tree;
}

/**
 * @brief Delete a tree and all its nodes.
 *
 * @param tree 
 */
void tree_delete(tree_t *tree)
{
	// Recursively destroy all nodes beneath root.
	if (tree->root)
		deletenode(tree->root);

	free(tree);
}

int tree_add(tree_t *tree, void *elem)
{
	node_t *curr = tree->root;

	if (!curr) {
		tree->root = newnode(elem);
		tree->size++;
		return 1;
	}

	int cmpval;

	while (curr) {
		cmpval = tree->cmp(curr->elem, elem);

		// If current elem < elem
		if (cmpval < 0) {

			// If current node does not have a left node,
			// assign value to a new node on its left.
			if (!curr->left) {
				curr->left = newnode(elem);
				tree->size++;
				return 1;
			}

			// If left node exist, go to left node and
			// start loop over again.
			curr = curr->left;
			continue;
		}

		// If current elem > elem
		if (cmpval > 0) {

			// create new node to curr's right if 
			// it does not have one.
			if (!curr->right) {
				curr->right = newnode(elem);
				tree->size++;
				return 1;
			}

			// Iterate from currents right node.
			curr = curr->right;
			continue;
		}
		DEBUG_PRINT("tree_add: Element already exist.");
		return 2;
	}
	return 0;
}

struct tree_iter
{
	node_t *root;
	void *start;
};

// IDEA: Add an extra field to the node_t struct that contains the next
//       sorted element in an iterable sense. I see that it is not
//       possible to do what was originally planned by using void
//       pointers to create an array for the elements as there is no
//       way to get the type and thereby size of each element.

tree_iter_t *tree_createiter(tree_t *tree)
{
	tree_iter_t *iter = malloc(sizeof(tree_iter_t));

	if (!iter)
		ERROR_PRINT("tree_createiter: Malloc failed!");

	iter->root = tree->root;

	return iter;
}

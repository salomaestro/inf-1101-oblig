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
typedef struct bnode bnode_t;
typedef struct bstack bstack_t;

struct tree {
	node_t *root;
	size_t size;
	cmpfunc_t cmp;
};

struct node {
	void *elem;
	node_t *left;
	node_t *right;
	node_t *parent;
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
	node->parent =NULL;

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
				curr->left->parent = curr;
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
				curr->right->parent = curr;
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

/**
* @brief Datatype implementation of tree_iter_t.
*
*/
struct tree_iter
{
	node_t *current;
	node_t *rightmost;
};

/**
 * @brief Get the leftmost node beneath @param root.
 *
 * @param root 
 * @return leftmost node.
 */
static node_t *node_leftmost(node_t *root)
{
	node_t *curr = root;

	while (curr->left) {
		curr = curr->left;
	}

	return curr;
}

/**
 * @brief Get the next node after the input node.
 *
 * @param node 
 * @return node
 */
static node_t *node_getnext(node_t *node)
{
	if (node->right) {
		return node_leftmost(node->right);
	}

	while (node->parent && node == node->parent->right) {
		node = node->parent;
	}

	return node->parent;
}

/**
 * @brief Create a iter to iterate over the input tree.
 *
 * @param tree 
 * @return iter
 */
tree_iter_t *tree_createiter(tree_t *tree)
{
	tree_iter_t *iter = malloc(sizeof(tree_iter_t));

	if (!iter)
		ERROR_PRINT("tree_createiter: Malloc failed!");

	iter->current = node_leftmost(tree->root);
	
	node_t *curr = iter->current;

	while (curr->right) {
		curr = curr->right;
	}

	iter->rightmost = curr;

	return iter;
}

/**
 * @brief Free the given iter.
 *
 * @param iter 
 */
void tree_destroyiter(tree_iter_t *iter)
{
	free(iter);
}

/**
 * @brief Check if current iter has a next value.
 *
 * @param iter 
 * @return 
 */
int tree_hasnext(tree_iter_t *iter)
{
	if (iter->current == NULL)
		return 0;
	return 1;
}

/**
 * @brief Returns the next element in the binary tree.
 *
 * @param iter 
 * @return elem
 */
void *tree_next(tree_iter_t *iter)
{
	node_t *used = iter->current;

	iter->current = node_getnext(used);

	if (iter->current == used) {
		iter->current = NULL;
		return NULL;
	}

	return used->elem;
}


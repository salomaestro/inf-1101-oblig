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
		ERROR_PRINT("newnode: Malloc failed!\n");

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
	if (!root) {
		INFO_PRINT("deletenode: Not a node...\n");
		return;
	}

	if (root->left) {
		INFO_PRINT("deletenode: Enter root->left...\n");
		deletenode(root->left);
	}

	if (root->right) {
		INFO_PRINT("deletenode: Enter root->right...\n");
		deletenode(root->right);
	}

	INFO_PRINT("deletenode: Freeing node\n");
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
		ERROR_PRINT("bst_create: Malloc failed!\n");

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
void tree_destroy(tree_t *tree)
{
	// Recursively destroy all nodes beneath root.
	if (tree->root)
		deletenode(tree->root);

	INFO_PRINT("tree_destroy: All subnodes destroyed.\n");
	free(tree);
}


/**
 * @brief Get the size (number of nodes) in the tree
 *
 * @param tree 
 * @return Number of elements in the tree.
 */
size_t tree_size(tree_t *tree)
{
	return tree->size;
}

/**
 * @brief Search tree for a element.
 *
 * @param tree 
 * @param elem 
 * @return 1 if element found. 0 otherwise.
 */
int tree_find(tree_t *tree, void *elem)
{
	node_t *curr = tree->root;

	int cmpval;

	while (curr) {
		cmpval = tree->cmp(curr->elem, elem);

		if (cmpval == 0) {
			INFO_PRINT("tree_find: Found element.\n");
			return 1;
		}

		if (cmpval > 0) {
			INFO_PRINT("tree_find: Accessing left subtree.\n");
			curr = curr->left;
			continue;
		} 
		if (cmpval < 0) {
			INFO_PRINT("tree_find: Accessing right subtree.\n");
			curr = curr->right;
			continue;
		}
	}
	INFO_PRINT("tree_find: Element not found.\n");
	return 0;
}


/**
 * @brief Add a element to the tree. The element must be a void *.
 *
 * @param tree 
 * @param elem 
 * @return 1 for success; element was added. 2 for element already exist;
 * the implementation does not allow for duplicate entries. 0 Something
 * unexpected happened; should in principle not return 0.
 */
int tree_add(tree_t *tree, void *elem)
{
	node_t *curr = tree->root;
	int cmpval;

	// Handle case where tree does not have a root.
	if (!curr) {
		tree->root = newnode(elem);
		tree->size++;
		return 1;
	}

	while (curr) {
		cmpval = tree->cmp(curr->elem, elem);

		// Indication to move to the left side of current node.
		if (cmpval > 0) {

			// If left node exist, go to left node and
			// start loop over again.
			if (curr->left) {
				curr = curr->left;
				continue;
				
			}

			// If current node does not have a left node,
			// assign value to a new node on its left.
			curr->left = newnode(elem);
			curr->left->parent = curr;
			tree->size++;
			return 1;
		}

		// Indicate to move to the left side of the current node.
		if (cmpval < 0) {

			if (curr->right) {

				// Iterate from currents right node.
				curr = curr->right; 
				continue;
			}

			// create new node to curr's right if 
			// it does not have one.
			curr->right = newnode(elem);
			curr->right->parent = curr;
			tree->size++;
			return 1;
		}
		INFO_PRINT("tree_add: Element already exist.\n");
		return 2;
	}
	DEBUG_PRINT("tree_add: Something unexpected happened.\n");
	return 0;
}

/**
 * @brief Copies a nodes properties, including all links to
 * other nodes and the element stored in the respective nodes.
 *
 * Note: Static function.
 *
 * @param node 
 * @return node_t *
 */
static node_t *nodecopy(node_t *node, node_t *parent)
{
	if (!node)
		return NULL;

	node_t *copy = newnode(node->elem);

	// Recursively set the right and left nodes.
	copy->right = nodecopy(node->right, copy);
	copy->left = nodecopy(node->left, copy);

	// set parent
	copy->parent = parent;

	return copy;
}


/**
 * @brief Create a shallow copy of the tree, meaning
 * the nodes are true new 'objects' but the elements
 * are clones and essentially the same as in the
 * original.
 *
 * @param tree 
 * @return tree_t *
 */
tree_t *tree_copy(tree_t *tree)
{
	tree_t *copy = tree_create(tree->cmp);

	copy->root = nodecopy(tree->root, NULL);
	copy->size = tree->size;

	return copy;
}


/**
 * @typedef Datatype implementation of tree_iter_t.
 *
 */
struct tree_iter
{
	node_t *current;
};

/**
 * @brief Get the leftmost node beneath @param root.
 *
 * @param root 
 * @return leftmost node.
 */
static node_t *node_leftmost(node_t *root)
{
	node_t *leftmost = root;

	while (leftmost->left) {
		leftmost = leftmost->left;
	}

	return leftmost;
}

/**
 * @brief Get the next node after the input node.
 *
 * @param node 
 * @return node
 */
static node_t *node_getnext(node_t *node)
{
	/*
	 * The following implementation is a tricky one, but stay frosty...
	 * To visit all elements in the tree in an ascending
	 * order; the first call to this function must have
	 * the leftmost node of the tree.
	 *
	 * The first conditional statement says that if the input
	 * node has a right node then find the leftmost node of
	 * the right node. This ensures that we always get the node
	 * with the smallest element first.
	 * 
	 * The while loop says that if the node has a parent, i.e.
	 * isn't the root node, and the current node is its own parents
	 * right node then set move to the parent node.
	 * This ensures that whenever we are in a subtree, the only way
	 * to move up is if we are one the rightmost path w.r.t the subtree.
	 *
	 */
	if (node->right)
		return node_leftmost(node->right);

	while (node->parent && node == node->parent->right)
		node = node->parent;

	return node->parent;
}

/**
 * @brief Create a iter to iterate over the input tree.
 *
 * @param tree 
 * @return iter | NULL (if tree is empty)
 */
tree_iter_t *tree_createiter(tree_t *tree)
{
	tree_iter_t *iter = malloc(sizeof(tree_iter_t));

	if (!iter)
		ERROR_PRINT("tree_createiter: Malloc failed!\n");

	// If the root of the tree is not defined, return an empty iterator.
	if (!tree->root) {
		iter->current = NULL;
		return iter;
	}

	// Start with the node that has the smallest value.
	iter->current = node_leftmost(tree->root);

	INFO_PRINT("tree_createiter: Tree iterator successfully created\n.");
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
	INFO_PRINT("tree_destroyiter: iter Successfully destroyed.\n");
}

/**
 * @brief Check if current iter has a next value.
 *
 * @param iter 
 * @return 0 (end of iterator) | 1 (next exist).
 */
int tree_hasnext(tree_iter_t *iter)
{
	if (iter->current)
		return 1;
	return 0;
}

/**
 * @brief Returns the next element in the binary tree.
 *
 * @param iter 
 * @return elem
 */
void *tree_next(tree_iter_t *iter)
{
	// Store the current element to output from iter's current element.
	node_t *used = iter->current;

	// Find the next element (will not be output in this iteration).
	iter->current = node_getnext(used);

	/*
	 * Because node_getnext(rightmost node) returns itself,
	 * we may use this as an indicator to stop the iteration
	 * when the current has been used.
	 */
	if (iter->current == used) {
		INFO_PRINT("tree_next: End of iterator\n.");
		iter->current = NULL;
		return NULL;
	}

	return used->elem;
}

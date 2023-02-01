#include <stdio.h>
#include "bst.h"
#include "common.h"

int main()
{
	tree_t *tree = tree_create(compare_strings);

	tree_add(tree, "Hello");
	tree_add(tree, "world!");

	// printf("%s\n", get_leftmost(tree));
}


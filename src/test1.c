#include <stdio.h>
#include "bst.h"
#include "common.h"
#include "printing.h"

int main()
{
	tree_t *tree = tree_create(compare_strings);

	tree_add(tree, "Hello");
	tree_add(tree, "world!");
	tree_add(tree, "I");
	tree_add(tree, "wow");
	tree_add(tree, "works...");

	printf("found :%d\n", tree_find(tree, "wow"));
	tree_iter_t *i = tree_createiter(tree);

	while(tree_hasnext(i)) {
		void *elem = tree_next(i);
		printf("%s\n", (char *)elem);
	}


	printf("Size of tree: %d\n", (int)tree_size(tree));
	// printf("Depth of tree: %d\n", tree_maxdepth(tree));

	DEBUG_PRINT("Gonna destroy tree iter.\n");
	tree_destroyiter(i);

	DEBUG_PRINT("Gonna destroy tree.\n");
	tree_destroy(tree);

}


#include <stdlib.h>
#include "bst.h"
#include "common.h"
#include "set.h"
#include "printing.h"

struct set
{
	tree_t *tree;
	cmpfunc_t cmpfunc;
};


set_t *set_create(cmpfunc_t cmpfunc)
{
	set_t *set = malloc(sizeof(set_t));

	if (!set)
		ERROR_PRINT("set_create: Malloc failed!\n");

	set->tree = tree_create(cmpfunc);
	set->cmpfunc = cmpfunc;

	return set;
}

void set_destroy(set_t *set)
{
	tree_destroy(set->tree);
	free(set);
	INFO_PRINT("set_destroy: Set successfully destroyed.\n");
}

int set_size(set_t *set)
{
	return (int)tree_size(set->tree);
}


void set_add(set_t *set, void *elem)
{
	tree_add(set->tree, elem);
}



int set_contains(set_t *set, void *elem)
{
	return tree_find(set->tree, elem);
}

set_t *set_union(set_t *a, set_t *b)
{
	void *elem;
	set_t *new_set = set_copy(a);

	set_iter_t *iter = set_createiter(b);

	while (set_hasnext(iter)) {
		elem = set_next(iter);

		// Does not need to check if element exist as
		// that will be handled by set_add.
		set_add(new_set, elem);
	}

	set_destroyiter(iter);

	INFO_PRINT("set_union: Created a union set.\n");
	return new_set;
}


set_t *set_intersection(set_t *a, set_t *b)
{
	set_t *new_set = set_create(a->cmpfunc);

	set_iter_t *iter = set_createiter(b);
	void *elem;

    /*
     * As we want to find overlaps between a and b,
     * we only need to iterate over one of them, 
     * and see which elements from one is in the other. 
     */
	while (set_hasnext(iter)) {
		elem = set_next(iter);

		if (set_contains(a, elem))
			set_add(new_set, elem);
	}

	INFO_PRINT("set_intersection: Created a intersection set.\n");
	return new_set;
}


set_t *set_difference(set_t *a, set_t *b)
{
	// Create a new empty set.
	set_t *new_set = set_create(a->cmpfunc);

	/* 
	 * Iterate over the other set.
	 */
	set_iter_t *iter = set_createiter(a);
	void *elem;

	while (set_hasnext(iter)) {
		elem = set_next(iter);

		// If element from b is not contained in a add to new set.
		if (!set_contains(b, elem))
			set_add(new_set, elem);
	}

	INFO_PRINT("set_difference: Created a difference set.\n");
	return new_set;
}


set_t *set_copy(set_t *set)
{
	set_t *copy = set_create(set->cmpfunc);

	copy->tree = tree_copy(set->tree);

	INFO_PRINT("set_copy: Created a set copy.\n");
	return copy;
}


struct set_iter
{
	tree_iter_t *tree_iter;
};



set_iter_t *set_createiter(set_t *set)
{
	set_iter_t *iter = malloc(sizeof(set_iter_t));

	if (!iter)
		ERROR_PRINT("set_createiter: Malloc failed!\n");

	iter->tree_iter = tree_createiter(set->tree);

	INFO_PRINT("set_createiter: Successfully created a set iterator.\n");
	return iter;
}


void set_destroyiter(set_iter_t *iter)
{
	tree_destroyiter(iter->tree_iter);
	free(iter);
	INFO_PRINT("set_destroyiter: Successfully destroyed set.\n");
}



int set_hasnext(set_iter_t *iter)
{
	return tree_hasnext(iter->tree_iter);
}



void *set_next(set_iter_t *iter)
{
	return tree_next(iter->tree_iter);
}

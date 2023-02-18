#include "set.h"
#include "list.h"
#include "printing.h"
#include "common.h"

#include <stdlib.h>

struct set 
{
    list_t *list;
    cmpfunc_t cmpfunc;
};

set_t *set_create(cmpfunc_t cmpfunc) 
{
    set_t *set = malloc(sizeof(set_t));

    if (!set) ERROR_PRINT("set_create: Malloc failed!\n");

    set->list = list_create(cmpfunc);
    set->cmpfunc = cmpfunc;

    INFO_PRINT("set_create: Success.\n");

    return set;
}

void set_destroy(set_t *set) 
{

    /* Destroy the list */
    list_destroy(set->list);

    /* Free the set */
    free(set);

    INFO_PRINT("set_destroy: Success.\n");
}

int set_size(set_t *set) 
{
    return list_size(set->list);
}

void set_add(set_t *set, void *elem) 
{

    list_t *list = set->list;

    if (list_contains(list, elem)) {
        INFO_PRINT("set_add: Elem already exist!\n");
        return;
    }

    if (!list_addlast(list, elem)) 
        ERROR_PRINT("set_add: Not able to insert elem to list!\n");
}

int set_contains(set_t *set, void *elem) 
{
    
    if (list_contains(set->list, elem))
        return 1;

    /* If not in set. */
    return 0;
}

set_t *set_union(set_t *a, set_t *b) 
{

    /* Verify that cmpfunc's are equal. */
    // if (a->cmpfunc != b->cmpfunc)
    //     ERROR_PRINT("set_union: Set a and b does not have same cmp function!\n");

    /* 
     * Create a new set that will contain the
     * elements from set a and set b.
     */
    set_t *new_set = set_create(a->cmpfunc);

    /* Iterators for both sets. */
    set_iter_t *iter_a = set_createiter(a);
    set_iter_t *iter_b = set_createiter(b);
    void *elem;

    /* Iterate over both sets and add the elem's. */
    while (set_hasnext(iter_a)) {
        elem = set_next(iter_a);
        set_add(new_set, elem);
    }
    
    INFO_PRINT("set_union: Iterated over A.\n");

    while (set_hasnext(iter_b)) {
        elem = set_next(iter_b);
        set_add(new_set, elem);
    }

    INFO_PRINT("set_union: Iterated over B.\n");

    /* Free space of iterators. */
    set_destroyiter(iter_a);
    set_destroyiter(iter_b);

    return new_set;
}

set_t *set_intersection(set_t *a, set_t *b) 
{
    /* Verify cmpfuncs are equal. */
    // if (a->cmpfunc != b->cmpfunc)
    //     ERROR_PRINT("set_intersection: Set a and b does not have same cmp function!\n");

    /* Create a new set which will be the intersection set. */
    set_t *new_set = set_create(a->cmpfunc);

    set_iter_t *iter = set_createiter(a);
    void *elem;

    /*
     * As we want to find overlaps between a and b,
     * we only need to iterate over one of them, 
     * and see which elements from one is in the other. 
     */
    while (set_hasnext(iter)) {
        elem = set_next(iter);

        if (set_contains(b, elem)) {
            set_add(new_set, elem);

            INFO_PRINT("set_intersection: Set B contained elem from set A.\n");
        }
    }

    set_destroyiter(iter);

    return new_set;
}

set_t *set_difference(set_t *a, set_t *b)
{
    // if (a->cmpfunc != b->cmpfunc)
    //     ERROR_PRINT("set_difference: Set a and b does not have same cmp function!\n");

    set_t *new_set = set_create(a->cmpfunc);

    set_iter_t *iter = set_createiter(a);
    void *elem;

    while (set_hasnext(iter)) {
        elem = set_next(iter);

        /* Negated the logic from set_intersection. */
        if (!set_contains(b, elem)) {
            set_add(new_set, elem);
        
            INFO_PRINT("set_difference: Found difference in A from B.\n");
        }
    }

    set_destroyiter(iter);

    return new_set;
}

set_t *set_copy(set_t *set)
{
    set_t *copy = set_create(set->cmpfunc);

    set_iter_t *iter = set_createiter(set);
    
    /*
     * The generic way to create a copy would be to
     * create a set, and iterate over the source set,
     * and use set_insert to copy elements from source
     * to destination.
     * However the source set can be regarded as being
     * sorted by default. Therefore we instead insert
     * elements directly at the end of the new set,
     * without sorting for each set_add operation.
     */
    void *elem;
    while (set_hasnext(iter)) {
        elem = set_next(iter);
        list_addlast(copy->list, elem);
    }

    set_destroyiter(iter);

    return copy;
}

struct set_iter 
{
    list_iter_t *list_iter;
};

set_iter_t *set_createiter(set_t *set) 
{
    set_iter_t *iter = malloc(sizeof(set_iter_t));
    
    if (!iter) ERROR_PRINT("set_createiter: Malloc failed!\n");
    
    list_sort(set->list);

    iter->list_iter = list_createiter(set->list);

    INFO_PRINT("set_createiter: Success.\n");
    return iter;
}

void set_destroyiter(set_iter_t *iter) 
{
    list_destroyiter(iter->list_iter);
    free(iter);
    INFO_PRINT("set_destroyiter: Success.\n");
}

int set_hasnext(set_iter_t *iter) 
{
    return list_hasnext(iter->list_iter);
}

void *set_next(set_iter_t *iter)
{
    return list_next(iter->list_iter);
}

static void printset(char *name, set_t *set) 
{
    printf("%s:\n", name);
    
    if (!set_size(set))
        printf("\tØ\n");

    list_iter_t *iter = list_createiter(set->list);
    while (list_hasnext(iter)) {
        printf("\t%d\n", *(int *)list_next(iter));
    }
}

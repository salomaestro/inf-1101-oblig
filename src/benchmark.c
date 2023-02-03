#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

#define TEST_SEED 0xabcd
// #define TEST_MODULUS ( TEST_SET_SIZE )

int compare_ints(void *a, void *b)
{
    int *ia = a;
    int *ib = b;

    return (*ia)-(*ib);
}

static void *newint(int i)
{
    int *p = malloc(sizeof(int));
    *p = i;
    return p;
}

/*
 * Generates a set from a seed value
 */

set_t *generate_set(unsigned int seed, int num)
{
	set_t *a;
	int i;
	
	a = set_create(compare_ints);
	
	/* Adding random numbers based on the seed value */
	for(i = 0; i < num; i++)
	{
		set_add(a, newint(rand_r(&seed) % num));
	}
	
	return a;
}

/*
 * Deletes a set
 */

void delete_generated_set(set_t *set)
{
	set_iter_t *iter;
	int *elem;
	
	/* Validate the result sets */
	iter = set_createiter(set);
	while(set_hasnext(iter))
	{
		elem = (int *)set_next(iter);
		free(elem);
	}
	
	set_destroyiter(iter);
	set_destroy(set);
}


int main(int argc, char *argv[])
{
	set_t *s = generate_set(TEST_SEED, 10);	


	delete_generated_set(s);
	return 0;
}

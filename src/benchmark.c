/* Author: Steffen Viken Valvaag <steffenv@cs.uit.no> */
#include "set.h"
#include <stdlib.h>
#include <time.h>

#include "printing.h"

static int compare_ints(void *a, void *b)
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
 * Print a set of int elements.
 */
static void printset(char *prefix, set_t *set)
{
    set_iter_t *it;

    INFO_PRINT("%s", prefix);
    it = set_createiter(set);
    while (set_hasnext(it)) {
	    int *p = set_next(it);
	    printf(" %d", *p);
    }
    printf("\n");
    set_destroyiter(it);
}

/*
 * Print a set of int elements, then destroy it.
 */
static void dumpset(char *prefix, set_t *set)
{
    // printset(prefix, set);
    set_destroy(set);
}

static void testset(set_t *set_a, set_t *set_b, int n) {
    clock_t start, end;
    double cpu_time_used;
    set_t *uni, *diff, *inter;

    start = clock();
    uni = set_union(set_a, set_b);
    printf("%d,union,%lf\n", n, ((double) (clock() - start) / CLOCKS_PER_SEC));

    start = clock();
    diff = set_difference(set_a, set_b);
    printf("%d,difference,%lf\n", n, ((double) (clock() - start) / CLOCKS_PER_SEC));

    start = clock();
    inter = set_intersection(set_a, set_b);
    printf("%d,intersection,%lf\n", n, ((double) (clock() - start) / CLOCKS_PER_SEC));

    set_destroy(uni);
    set_destroy(diff);
    set_destroy(inter);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
	ERROR_PRINT("Must provide exactly 1 argument!\n");
    }

    set_t *set_a, *set_b, *set_c, *set_d;
    int n_tests, i, j, n = 10000;
    int **nums;

    n_tests = atol(argv[1]);

    srand(time(NULL));

    /* Allocate numbers for different tests. */
    nums = (int **) malloc(sizeof(int*) * n);

    for (i = 0; i < n; i++) {
	nums[i] = newint(rand() % n);
    }

    set_a = set_create(compare_ints);
    set_b = set_create(compare_ints);

    for (j = 0; j < n_tests; j++) {
	for (i = 0; i < n; i++) {
	    set_add(set_a, nums[rand() % n]);
	    set_add(set_b, nums[rand() % n]);

	    if (i == n/100 - 1) {
		testset(set_a, set_b, n/100);
	    }
	    if (i == n/10 - 1) {
		testset(set_a, set_b, n/10);
	    }
	    if (i == n - 1) {
		testset(set_a, set_b, n);
	    }

	}
    }

    set_destroy(set_a);
    set_destroy(set_b);

    for (i = 0; i < n; i++) {
	free(nums[i]);
    }
    free(nums);
}

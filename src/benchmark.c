/* Author: Steffen Viken Valvaag <steffenv@cs.uit.no> */
#include <stdlib.h>
#include <time.h>
#include "set.h"
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


static double timesince(clock_t start)
{
    return (double) (clock() - start) / CLOCKS_PER_SEC;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
	ERROR_PRINT("Must provide exactly 1 argument!\nUsage: %s 100\n", argv[0]);
    }

    set_t *set_a, *set_b, *union_set, *intersection_set, *difference_set;
    clock_t start;
    double cumsum = 0;
    int i, n;
    int **nums;

    n = atol(argv[1]);

    srand(time(NULL));

    /* Allocate numbers for different tests. */
    nums = (int **) malloc(sizeof(int*) * n);

    for (i = 0; i < n; i++) {
	nums[i] = newint(rand() % n);
    }

    set_a = set_create(compare_ints);
    set_b = set_create(compare_ints);

    printf("n,add,union,difference,intersection\n");

    for (i = 0; i < n; i = i + 1) {
	start = clock();
	set_add(set_a, nums[rand() % n]);
	// printf("%d,add,%lf\n", i, timesince(start));
	cumsum += timesince(start);

	printf("%d,%lf,", i, cumsum);

	set_add(set_b, nums[rand() % n]);

	start = clock();
	union_set = set_union(set_a, set_b);
	// printf("%d,union,%lf\n", i, timesince(start));
	printf("%lf,", timesince(start));

	start = clock();
	difference_set = set_difference(set_a, set_b);
	// printf("%d,difference,%lf\n", i, timesince(start));
	printf("%lf,", timesince(start));


	start = clock();
	intersection_set = set_intersection(set_a, set_b);
	// printf("%d,intersection,%lf\n", i, timesince(start));
	printf("%lf\n", timesince(start));

	set_destroy(union_set);
	set_destroy(difference_set);
	set_destroy(intersection_set);
    }

    set_destroy(set_a);
    set_destroy(set_b);

    for (i = 0; i < n; i++) {
	free(nums[i]);
    }
    free(nums);
}

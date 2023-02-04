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

static void testset(int setsize, int **nums) {

}

int main(int argc, char **argv)
{
    if (argc != 2) {
	ERROR_PRINT("Must provide exactly 1 argument!\n");
    }

    set_t *set_a, *set_b, *set_c, *set_d;
    int n_tests, i, r, n = 10000;
    int **num_e4, **num_e3, **num_e2;

    n_tests = atol(argv[1]);

    srand(time(NULL));

    /* Allocate numbers for different tests. */
    num_e4 = (int **) malloc(sizeof(int*) * n);
    num_e3 = (int **) malloc(sizeof(int*) * n/10);
    num_e2 = (int **) malloc(sizeof(int*) * n/100);

    for (i = 0; i < n; i++) {
	num_e4[i] = newint(rand() % n);
	
	if (i < n/10) {
	    num_e3[i] = newint(rand() % n/10);
	}

	if (i < n/100) {
	    num_e2[i] = newint(rand() % n/100);
	}
    }

    /* Perform tests on the different set sizes */
    for (i = 0; i < n_tests; i++) {
	testset(n, num_e4);
	testset(n/10, num_e3);
	testset(n/100, num_e2);
    }


    for (i = 0; i < n; i++) {
	free(num_e4[i]);

	if (i < n/10) {
	    free(num_e3[i]);
	}

	if (i < n/100) {
	    free(num_e2[i]);
	}
    }

    free(num_e4);
    free(num_e3);
    free(num_e2);

  //   set_t *all, *evens, *odds, *nonprimes, *primes;
  //   int i, j, n = 1000;
  //   int **numbers;
		//
		//
  //   /* Create sets */
  //   all = set_create(compare_ints);
  //   evens = set_create(compare_ints);
  //   odds = set_create(compare_ints);
  //   nonprimes = set_create(compare_ints);
  //   primes = set_create(compare_ints);
		//
  //   /* Initialize sets */
  //   for (i = 0; i <= n; i++) {
		// set_add(all, numbers[i]);
	 //    if (i % 2 == 0) {
	 //        set_add(evens, numbers[i]);
	 //    }
	 //    else {
	 //        set_add(odds, numbers[i]);
	 //    }
  //       if (i < 2) {
  //           set_add(nonprimes, numbers[i]);
  //       }
  //       else {
	 //        for (j = i+i; j <= n; j += i) {
	 //            set_add(nonprimes, numbers[j]);
	 //        }
  //       }
	 //    if (!set_contains(nonprimes, numbers[i])) {
	 //        set_add(primes, numbers[i]);
	 //    }
  //   }
		//
  //   /* Show resulting sets */
  //   // printset("Numbers:", all);
  //   // printset("Even numbers:", evens);
  //   // printset("Odd numbers:", odds);
  //   // printset("Non-prime numbers:", nonprimes);
  //   // printset("Prime numbers:", primes);
		//
  //   /* Test unions */
  //   dumpset("Even or odd numbers:", set_union(evens, odds));
  //   dumpset("Prime or non-prime numbers:", set_union(primes, nonprimes));
  //   dumpset("Even or prime numbers:", set_union(evens, primes));
  //   dumpset("Odd or prime numbers:", set_union(odds, primes));
		//
  //   /* Test intersections */
  //   dumpset("Even and odd numbers:", set_intersection(evens, odds));
  //   dumpset("Even non-prime numbers:", set_intersection(evens, nonprimes));
  //   dumpset("Odd non-prime numbers:", set_intersection(odds, nonprimes));
  //   dumpset("Odd prime numbers:", set_intersection(odds, primes));
  //   dumpset("Even prime numbers:", set_intersection(evens, primes));
		//
  //   /* Test differences */
  //   dumpset("Even non-prime numbers:", set_difference(evens, primes));
  //   dumpset("Odd non-prime numbers:", set_difference(odds, primes));
  //   dumpset("Even prime numbers:", set_difference(evens, nonprimes));
  //   dumpset("Odd prime numbers:", set_difference(odds, nonprimes));
		//
  //   /* Cleanup */
  //   set_destroy(all);
  //   set_destroy(evens);
  //   set_destroy(odds);
  //   set_destroy(nonprimes);
  //   set_destroy(primes);
		//
  //   for (i = 0; i <= n; i++) {
	 //    free(numbers[i]);
  //   }
  //   free(numbers);
}

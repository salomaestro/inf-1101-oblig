## Author: Steffen Viken Valvaag <steffenv@cs.uit.no> 
LIST_SRC=linkedlist.c
SET_SRC=set.c
BST_SRC=bst.c
SET_BST_SRC=set_bst.c
EZ_SRC=$(LIST_SRC) $(SET_SRC)
BB_SRC=$(BST_SRC) $(SET_BST_SRC) $(LIST_SRC)

SPAMFILTER_SRC=spamfilter.c common.c
NUMBERS_SRC=numbers.c common.c
BENCHMARK_SRC=benchmark.c common.c
ASSERT_SRC=assert_set.c common.c

INCLUDE=include

BENCHMARK_SRC:=$(patsubst %.c,src/%.c, $(BENCHMARK_SRC) $(EZ_SRC))
NUMBERS_SRC:=$(patsubst %.c,src/%.c, $(NUMBERS_SRC) $(BB_SRC))
SPAMFILTER_SRC:=$(patsubst %.c,src/%.c, $(SPAMFILTER_SRC) $(BB_SRC))
ASSERT_EZ_SRC:=$(patsubst %.c,src/%.c, $(ASSERT_SRC) $(EZ_SRC))
ASSERT_BB_SRC:=$(patsubst %.c,src/%.c, $(ASSERT_SRC) $(BB_SRC))

CFLAGS=-Wall -Wextra -g -Wpedantic -O0
LDFLAGS=-lm -DLOG_LEVEL=1 -DERROR_FATAL

all: spamfilter numbers

spamfilter: $(SPAMFILTER_SRC) Makefile
	gcc -o $@ $(CFLAGS) $(SPAMFILTER_SRC) -I$(INCLUDE) $(LDFLAGS)

numbers: $(NUMBERS_SRC) Makefile
	gcc -o $@ $(CFLAGS) $(NUMBERS_SRC) -I$(INCLUDE) $(LDFLAGS)

benchmark: $(NUMBERS_SRC) Makefile
	gcc -o $@ $(CFLAGS) $(BENCHMARK_SRC) -I$(INCLUDE) $(LDFLAGS)

assertez: $(ASSERT_EZ_SRC) Makefile
	gcc -o $@ $(CFLAGS) $(ASSERT_EZ_SRC) -I$(INCLUDE) $(LDFLAGS)

assertbb: $(ASSERT_BB_SRC) Makefile
	gcc -o $@ $(CFLAGS) $(ASSERT_BB_SRC) -I$(INCLUDE) $(LDFLAGS)

clean:
	rm -f *~ *.o *.exe spamfilter numbers assertbb assertez benchmark&& rm -rf *.dSYM

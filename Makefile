IMPLEMENTATION=list

ifeq ($(IMPLEMENTATION),list)
  SRC=linkedlist.c set.c
  SPAMFILTER_SRC=spamfilter.c common.c
else ifeq ($(IMPLEMENTATION),bst)
  SRC=bst.c set_bst.c
  SPAMFILTER_SRC=spamfilter.c common.c linkedlist.c
endif

NUMBERS_SRC=numbers.c
BENCHMARK_SRC=benchmark.c
ASSERT_SRC=assert_set.c

INCLUDE=include

BENCHMARK_SRC:=$(patsubst %.c,src/%.c, $(BENCHMARK_SRC) $(SRC))
NUMBERS_SRC:=$(patsubst %.c,src/%.c, $(NUMBERS_SRC) $(SRC))
SPAMFILTER_SRC:=$(patsubst %.c,src/%.c, $(SPAMFILTER_SRC) $(SRC))
ASSERT_SRC:=$(patsubst %.c,src/%.c, $(ASSERT_SRC) $(SRC))

CFLAGS=-Wall -Wextra -g -Wpedantic #-O0
LDFLAGS=-lm -DLOG_LEVEL=1 -DERROR_FATAL

all: spamfilter numbers

spamfilter: $(SPAMFILTER_SRC) Makefile
	gcc -o $@ $(CFLAGS) $(SPAMFILTER_SRC) -I$(INCLUDE) $(LDFLAGS)

numbers: $(NUMBERS_SRC) Makefile
	gcc -o $@ $(CFLAGS) $(NUMBERS_SRC) -I$(INCLUDE) $(LDFLAGS)

benchmark: $(NUMBERS_SRC) Makefile
	gcc -o $@ $(CFLAGS) $(BENCHMARK_SRC) -I$(INCLUDE) $(LDFLAGS)

assert: $(ASSERT_SRC) Makefile
	gcc -o $@ $(CFLAGS) $(ASSERT_SRC) -I$(INCLUDE) $(LDFLAGS)

clean:
	rm -f *~ *.o *.exe spamfilter numbers assert benchmark && rm -rf *.dSYM


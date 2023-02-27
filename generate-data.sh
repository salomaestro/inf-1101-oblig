#!/bin/bash

# Where to store the data
OUTBST=benchmarks/2500_bst.csv
OUTLIST=benchmarks/2500_list.csv

mkdir -p benchmarks

# Compile benchmark binary with linked list backend
make benchmark

# Benchmark with 2500 elements and store to list-csv
./benchmark 2500 > $OUTLIST

echo "Done with linked list benchmark..."

make clean

# Compile benchmark binary with linked bst backend
make IMPLEMENTATION=bst benchmark

# Benchmark with 2500 elements and store to bst-csv
./benchmark 2500 > $OUTBST

make clean

echo "Done..."

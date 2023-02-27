# inf-1101-oblig
My implementation of the problems given in the mandatory assignment in UiT 
course inf-1101. Be aware that the functionality provided in this assignment
is intended for use on unix systems.

## How to use

The `Makefile` of the project contain both the original pre-code keywords, and
some new ones.

In accordance with the reqirements of the assignment, there are two 
implementations for the `set`. To compile `all`, `spamfilter`, `numbers`, 
`benchmark` or `assert`, with either the binary search tree or the linked list 
backend, use the argument `IMPLEMENTATION=bst` in the command:

```bash
make IMPLEMENTATION=bst benchmark
```

You do not need to specify `IMPLEMENTATION=list` as this is the default 
argument.

### Verifying spamfilter and numbers

Use the Make command:

```bash
make equal
```

This will compile `spamfilter.c` and `numbers.c`, execute both and store the
output into textfiles `spamfilter-got.txt` and `numbers-got.txt`. Then a Bash
script `equality.sh` will filter the outputs and compare them.

### Reproduce plots from report

Use the command:

```bash
make plot
```

This will create a python virtual environment, install requirements from 
`python-requirements.txt` and finally store all plots in `plots/`.

#### Regenerate data for plotting

Note that the code will be handed in with data already made. To regenerate it
use:

```bash
make gendata
```

This command will generate the data for both bst and linked list 
implementations.

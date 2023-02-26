# inf-1101-oblig
My implementation of the problems given in the mandatory assignment in UiT course inf-1101.

## TODO

- [ ] Fix makefile to spawn venv, install dependencies under `make install`.
- [ ] Check that `python-requirements.txt` is up to date.
- [ ] Finish `equalith.sh`, it should both test `number.c` and `spamfilter.c`.
- [ ] Create proper `README.md`.
- [ ] Double check that all assignment criteria has been met.


## A note on checking the correct output

We are given a file `numbers.c` which use the `set` implementation found in either `set.c` or `set_bst.c`, to verify that our set implementation works as it should.

The file `numbers-expected.txt` provide what the output when compiling and executing the contents of `numbers.c` ought to be. To quickly verify this (not having to manually look over all numbers) we can use one of the GNU library functions `awk` and `diff`. Or execute `numbers-equality.sh`.

### How to verify
1. Compile `numbers.c` using `make numbers`.
2. Put output of execution in a file e.g `numbers-out.txt` using:
```bash
./numbers > numbers-out.txt
```
3. Format `numbers-expected.txt` to the same format using `awk`:
```sh
awk -F: '{print $2}' numbers-expected.txt > numbers-expected-filtered.txt
```
Note that the `-F` flag sets the field separator to be `:`, then we `print $2` meaning the second argument from `src.txt > dst.txt`.
4. Check for equality using `diff`:
```sh
diff numbers-expected-filtered.txt numbers-out.txt
```
This compares the two files, line by line and exits with code `0` on equality.

##### (Create a shell script for this?)

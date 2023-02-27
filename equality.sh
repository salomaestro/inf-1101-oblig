#!/bin/bash

# Read target files from command line
TARGETNUMBERS=$1
TARGETSPAM=$2

# Store filenames of files with expected output.
EXPECTEDNUMBERS=./numbers-expected.txt
EXPECTEDSPAM=./spamfilter-expected.txt

# Temporary file
TEMPNUMBERS=./tempnums.txt

# Put all files into list
FILES=($TARGETNUMBERS $EXPECTEDNUMBERS $TARGETSPAM $EXPECTEDSPAM)

ALL_IS_GOOD=1

# Verify all files exist.
for file in "${FILES[@]}"; do
	if ! test -f $file; then
		echo "Missing file: ${file}."
		ALL_IS_GOOD=0
	fi
done

# Exit if not all files are present.
if [[ $ALL_IS_GOOD == 0 ]]; then
	echo "Use: make equal"
	exit;
fi

# Create temporary file
touch $TEMPNUMBERS

# Delimit on : and write output to temporary file
awk -F: '{print $2}' $EXPECTEDNUMBERS> $TEMPNUMBERS

# Compare temporary file with target
diff -s $TEMPNUMBERS $TARGETNUMBERS

# Function which filters to outputs from spamfilter because produced
# output is not necessarily ordered by mailx.txt, ordering is arbitrary.
filterspam () {
	cat $1 | awk -F'mail' '{print $3}' | awk -F. '{print $1" "$2}' | awk '{print $1" "$3" "$7}' | sort -nk1
}

# Store function output into variables
v1=$(filterspam $TARGETSPAM)
v2=$(filterspam $EXPECTEDSPAM)

# Test if outputs are equal.
test "$v1" = "$v2" && echo "Files $TARGETSPAM and $EXPECTEDSPAM produce same output"

echo "Cleaning up tempfile..."
rm $TEMPNUMBERS

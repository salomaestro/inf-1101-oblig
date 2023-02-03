#!/bin/bash

TARGET=./numbers-out.txt
TRUTHFILE=./numbers-expected.txt
TEMPFILE=./temp.txt

# Verify that target exists
if ! test -f "$TARGET"; then
	echo "$TARGET does not exist."

	if ! test -f ./numbers; then
		echo "Executable does not exist."


		while true; do
			read -r -p "Build? (Y/n): " ans
			case $ans in
				[Yy]* ) make numbers; break;;
				[Nn]* ) exit;;
				* ) echo "Try again..:";;
			esac
		done
	fi

	while true; do
		read -r -p "Run? (Y/n):" ans2
		case $ans2 in
			[Yy]* ) ./numbers > $TARGET; break;;
			[Nn]* ) exit;;
			* ) echo "Try again..:";;
		esac
	done
fi


if ! test -f "$TARGET"; then
	echo "$TARGET does not exist."
else
	echo "Successully built and ran $TARGET"
fi


awk -F: '{print $2}' $TRUTHFILE > $TEMPFILE

diff -s $TEMPFILE $TARGET

_="$(make clean)"

rm $TEMPFILE $TARGET

#!/bin/bash
echo "Running Tests..."
fail=0

# Test default and -P functionality
FILES=testDataA3/*.c-
for f in $FILES
do
    # default
    compare=${f/.c-/.out}
    echo ${f/.c-/}
    DIFF="$(diff <(cat $compare) <(./c- "$f"))"

    if [ "$DIFF" != "" ]
    then
        echo "ERROR: mismatch in \"DIFF ($compare) (./c- $f)\":"
        echo "$DIFF"
        printf "\n"
        fail=1
    fi

    # -P
    compare=${f/.c-/.Pout}
    echo ${f/.c-/}
    DIFF="$(diff <(cat $compare) <(./c- -P "$f"))"

    if [ "$DIFF" != "" ]
    then
        echo "ERROR: mismatch in \"DIFF ($compare) (./c- -P $f)\":"
        echo "$DIFF"
        printf "\n"
        fail=1
    fi
done


if [ "$fail" == "1" ]
then
    exit 1
fi

echo "Tests Passed"
exit 0
#!/bin/bash
echo "Running Tests..."
fail=0

# Test default and -P functionality
FILES=testDataA3/*.c-
for f in $FILES
do
    # default
    compare=${f/.c-/.out}
    command="diff <(cat $compare) <(./c- "$f")"
    printf "$command"
    DIFF="$(diff <(cat $compare) <(./c- "$f"))"

    if [ "$DIFF" != "" ]
    then
        echo -e "\tFAILED:"
        echo "$DIFF"
        fail=$((fail+1))
    fi

    printf "\n"

    # -P
    compare=${f/.c-/.Pout}
    command="diff <(cat $compare) <(./c- -P "$f")"
    printf "$command"
    DIFF="$(diff <(cat $compare) <(./c- -P "$f"))"

    if [ "$DIFF" != "" ]
    then
        echo -e "\tFAILED:"
        echo "$DIFF"
        fail=$((fail+1))
    fi

    printf "\n"
done


if ((fail>0)) 
then
    echo "TESTS FAILED: $fail"
    exit 1
fi

echo "Tests Passed"
exit 0
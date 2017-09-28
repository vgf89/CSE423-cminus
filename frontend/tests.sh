input='testDataA2/everything06.c-'
compare='testDataA2/everything06.out'
DIFF="$(diff <(cat $compare) <(./c- "$input"))"

if [ "$DIFF" != "" ]
then
    echo "Output Mismatch:\n"
    echo "$DIFF"
    exit 1
fi
exit 0
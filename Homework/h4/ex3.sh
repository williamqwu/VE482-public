#!/bin/bash
FILE=./ex3.out
# CNT=100
if ! test -f "$FILE"; then
    echo "0" >> $FILE
fi
for i in {1..20}
do
    operand=$(tail -n 1 $FILE)
    ((operand++))
    echo $operand >> $FILE
done

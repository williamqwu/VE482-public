#!/bin/bash
FILE=./ex3.out
if ! test -f "$FILE";
then
    echo "0" >> $FILE
fi
for i in {1..20}
do
(
    flock -n -x 33
    if [ $? -eq 1 ];
    then
        exit;
    fi
    operand=$(tail -n 1 $FILE)
    ((operand++))
    echo $operand >> $FILE
) 33>>$FILE
done

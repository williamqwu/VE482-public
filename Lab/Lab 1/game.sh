#!/bin/bash
key=$((($RANDOM)%100))
guess=999
while [ $key -ne $guess ] ; do
    read -p "Input a guessed number: " guess
    if [ $key -eq $guess ] ; then
        echo "Congratulates, you win!"
        break
    elif [ $key -gt $guess ] ; then
        echo "Larger"
    else
        echo "Smaller"
    fi
done

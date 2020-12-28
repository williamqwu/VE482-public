#!/bin/bash
for i in {1..20}
do
    # ./ex3.sh; ./ex3.sh&
    ./ex3_fix.sh; ./ex3_fix.sh&
done

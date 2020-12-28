# VE482 h3 ex2
## Input File Format
- `rand_int.txt`: the type of data is `int`
- `rand_char.txt`: the type of data is `char*`
- `rand_double.txt`: the type of data is `double`

## Output File Format
Format: `sortingtype_datatype.txt`

sortingtype:
- `rand`: the order of elements is random
- `inc`: the sorting order is increasing
- `dec`: the sorting order is decreasing

datatype:
- `int`: the type of data is `int`
- `char`: the type of data is `char*`
- `double`: the type of data is `double`

## Test

In order to run the test, first compile the program with

```
gcc ex2.c -o ex2
```

Next, run the program with two arguments:

```
./ex2 rand_int.txt dec
```


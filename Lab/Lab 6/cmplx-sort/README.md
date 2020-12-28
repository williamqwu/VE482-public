# Complex Sort
A software that sorts different types of file with a plugin architecture.

*Exercise 3 for VE482 Lab 6*

## build
```shell
make all
```

Note: 
- the inner file folder `plugins` is needed to hold the compiled plugins. Make sure it exists before compiling.
- the command `make all` will also run two testcases, one for `.txt` and another for `.csv`.
- a simple test command for txt is `./cmplxsort testcases/rand_int.txt inc`.
- a simple test command for csv is `./cmplxsort testcases/test.csv inc`.
- additional testcase should be placed under the sub-directory `testcases/`

## file structure
`src-kernel/*`: source files for the kernel program <br>
`src-plugin-txt/*`: source files for plugin that supports `.txt` <br>
`src-plugin-csv/*`: source files for plugin that supports `.csv` <br>
`Makefile`: make file for this project <br>
`plugins/*`: a folder that holds the compiled plugin programs <br>
`testcases/*`: a folder that holds some simple testcases <br>
`test-lib/*` / `test-plugin/*`: testing programs for dynamic libraries. Credit: https://hackaday.com/2018/07/12/its-all-in-the-libs-building-a-plugin-system-using-dynamic-loading/ <br>

## outlook
Some of the functions in `ui.c/h` can be moved to the kernel program, and the plugin function should also be updated correspondingly.

<div style="width:60%;height:200px;text-align:center;border:14px solid #808080;border-top:none;border-left:none;border-bottom:none;display:inline-block">
    <div style="border:4px solid #808080;border-radius:8px;width:95%;height:100%;background-color: rgb(209, 209, 209);">
        <div style="width:100%;height:30%;text-align:center;line-height:60px;font-size:26px;font-family:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;">VE482 Lab Report</div>
        <div style="width:100%;height:18%;text-align:center;line-height:26px;font-size:20px;font-familny:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;"><b>Lab 5</b> - Fall 2020</div>
        <div style="width:100%;height:57%;text-align:center;font-size:16px;line-height:22px;font-family: 'Courier New', Courier, monospace;font-weight:300;"><br><b>Name: Wu Qinhang<br>ID: 518370910041<br>Email: william_wu@sjtu.edu.cn<br></b></div>
    </div>
</div>
<div style="width:35%;height:200px;display:inline-block;float:right">
    <div style="width:100%;height:25%;text-align:center;line-height:55px;font-size:20px;font-family:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;"><b>Table of Contents</b></div>
    <div style="width:100%;height:75%;text-align:left;margin-left:2px;line-height:30px;font-size:13px;font-family:Verdana, Geneva, Tahoma, sans-serif;font-weight:300;">• Layer Programming<br>• Libraries</div>
</div>



[TOC]

## Layer Programming

**The program can be divided into three layers, what are they?**

- list structure
- sorting operations
- user interface

### File Structure

- `list.c/h`
  - list structure
- `sort.c/h`
  - sorting operations
- `ui.c/h`
  - user interface
- `driver_cmd.c`
  - main function for cmd user interface
- `driver_menu.c`
  - main function for menu user interface

### I/O File Format for cmd ui

Input:

- `rand_int.txt`: the type of data is `int`
- `rand_char.txt`: the type of data is `char*`
- `rand_double.txt`: the type of data is `double`

Output:

Format: `sortingtype_datatype.txt`

sortingtype:

- `rand`: the order of elements is random
- `inc`: the sorting order is increasing
- `dec`: the sorting order is decreasing

datatype:

- `int`: the type of data is `int`
- `char`: the type of data is `char*`
- `double`: the type of data is `double`

### Compile the Program

A `Makefile` is provided to compile the program. A sample command is provided below:

```shell
make cmd
./ui_cmd rand_int.txt dec
make menu
./ui_menu
```

A set of testcases is provided in the folder.

## Libraries

- **What are the three stages performed when compiling a file?**
  - preprocessing, compilation and linking
  - For preprocessing, the preprocessor includes other header files, expands macros, does conditional compilation and so on.
  - For compilation, the compiler read the source files and produce the corresponding assembly code.
  - For linking, the linker takes several object files and produce a single binary executable program.
- **difference between static and dynamic library**
  - static libraries are locked in to the program when being compiled; dynamic libraries exist as separate files outside the binary file.

### static library

use `-c` command to compile the source and header file so as to create a static library file.

create two static library files:

```shell
gcc -c list.c -o list.o
ar rcs list.a list.o 
gcc -c sort.c -o sort.o
ar rcs sort.a sort.o 
gcc -c ui.c -o ui.o
ar rcs ui.a ui.o 
```

compile the cmd driver program:

```shell
gcc -c driver_menu.c -o ui_cmd.o
gcc -o ui_cmd ui_cmd.o -L. -lsort -llist -lui
./ui_cmd
```

### dynamic library[^1]

```shell
gcc *.c -c -fPIC
gcc *.o -shared -o libmenu.so
gcc -L. -o ui_menu ui_menu.c -lmenu
export LD_LIBRARY_PATH=./:$LD_LIBRARY_PATH
./ui_menu
```

difference between a library and the API: 

An API is a software component that provides the developer with many applicable utilities, while a library is a collection of functionality that may not make up of an application.

For the implementation, see `./lab5_dlist.h`.

[^1]: "Shared libraries with GCC on Linux - Cprogramming.com," *Cprogramming.com*, 2019. https://www.cprogramming.com/tutorial/shared-libraries-linux-gcc.html (accessed Oct. 28, 2020).

## Known Issues

- the compile command for static/dynamic libraries
- the code for implementation of lab5_dlist (in major the implementation for sorting)
- the explanation between the library and the API
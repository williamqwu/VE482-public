# VE482 Homework 3

<center><div style="width:100%;height:57%;text-align:center;font-size:16px;line-height:25px;font-family: 'Courier New', Courier, monospace;font-weight:300;"><b>Due: Oct.18<br>Name: Wu Qinhang<br>ID: 518370910041<br>Email: william_wu@sjtu.edu.cn</b>
</div></center>

## Ex1 General Questions

### 1.1

**Why would a thread voluntarily release the CPU?**

Unlike processes, there doesn't exist clock interrupt. Since threads are working together for the tasks in the same process, it is better for threads to voluntarily release the CPU so as to let other threads run. 

### 1.2

**What is the biggest advantage/disadvantage of user space threads?**

Advantage: expansibility. The implementation of user space thread can also be implemented on another operating system that doesn't support threads.

Disadvantage: the process will be clocked if any of the thread blocks.

### 1.3

**If a multithreaded process forks, a problem occurs if the child gets copies of all the parent’s threads. Suppose that one of the original threads was waiting for keyboard input. Now two threads are waiting for keyboard input, one in each process. Does this problem ever occur in single-threaded processes?**

No, because in this situation, it will fail to fork.

### 1.4

**Many UNIX system calls have no Win32 API equivalents. For each such call, what are the consequences when porting a program from a UNIX system to a Windows system?**

Chances are that they cannot run properly.

## Ex2 C Programming

See `./README` and related source files.

## Ex3 Research on POSIX

POSIX, abbreviation for the Portable Operating System Interface, is an IEEE standard aiming at maintaining the compatibility and portability between different operating systems.[^1] Many API are defined in POSIX with variants of Unix and many other operating systems. Due to the various settings and implementation of different operating systems, it is always the case that applications on a certain OS may fail on another by system specific issues. POSIX is introduced so as to help developers port applications more simply. Apart from the application program interface, it also defines functions and facilities that are necessary in most popular commercial applications.

Generally there are four volumes in POSIX standard: base definitions, system interfaces, shell and utilities and "rationale" which includes explanations and some historical information. In base definitions, general terms, concepts, C header file specifications and some conventions for utility are defined. In system interfaces, the prototypes for C language function are included. In shell and utilities, the names, options and operands of utility programs are included together with the description for corresponding behavior, portability and error handling.[^2]



## Reference

[^1]:“POSIX.1 FAQ,” *Opengroup.org*, 2020. [Online]. Available: http://www.opengroup.org/austin/papers/posix_faq.html. [Accessed: 15-Oct-2020].
[^2]:D. Heller, “CMPSC 311, The Posix Standard,” *Psu.edu*, 2010. [Online]. Available: http://www.cse.psu.edu/~deh25/cmpsc311/Lectures/Standards/PosixStandard.html. [Accessed: 15-Oct-2020].
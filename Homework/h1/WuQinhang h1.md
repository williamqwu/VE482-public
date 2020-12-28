# VE482 Homework 1

<center><div style="width:100%;height:57%;text-align:center;font-size:16px;line-height:25px;font-family: 'Courier New', Courier, monospace;font-weight:300;"><b>Due: Sep.20<br>Name: Wu Qinhang<br>ID: 518370910041<br>Email: william_wu@sjtu.edu.cn</b>
</div></center>

## Ex1. Revisions

As far as memory is concerned, stack allocation and heap allocation are two ways of allocating memory in the computer. Stack allocation is based on function call stack and it allocates the memory for variables when the program is compiled. During Heap allocation, the memory of variables are allocated during the execution of corresponding instructions.

|                 | stack allocation | heap allocation |
| --------------- | ---------------- | --------------- |
| category        | static           | dynamic         |
| allocation type | automatic        | manual          |
| access speed    | fast             | slow            |
| limited size    | small            | big             |

## Ex2. Person research

### 2.1

When the computer is powered on, BIOS will first be launched to ensure that the operating system runs correctly. It will launch the boot loader, which will run a series of operations including POST and MBR. POST stands for Power on Self Test, which examine various hardware devices of the computer, including RAM, CPU, Video Card and so on. MBR stands for Master Boot Record, which searches for the existing operating system in the computer. After that, the operating system will be initialized, and the control of all the hardware will be handed over to the operating system.

During that process, BIOS is responsible for running hardware tests, searching for existing operating system and initialize it. [^1]

### 2.2

Hybrid kernel is a combinational design of microkernel and monolithic kernel that is both stable and efficient. It maintains device drives, which makes it more user-friendly.

Exo kernel is a design of operating system architecture that separate resource protection from its management so as to take advantage of hardware resources more efficiently and reduce the pressure of testing new functions of the operating system. [^2]

## Ex3

### 3.1

**a), c), d).**

Generally, kernel mode is reserved for trusted functions which have access to all of the hardware, while user mode comes along with protection and isolation. If interrupt disabling, clock setting and memory map management is available in user mode, chances are that crashes may happen which are not recoverable. Therefore, a, c, d should only be restricted in kernel mode. However, reading the time-of-day clock does nothing to the operating system since it is read-only. Therefore, it can be available in user mode.

### 3.2

**20ms or 25ms or 30ms or 35ms.**

When P0/P1 is assigned to CPU1 while P2 is assigned to CPU2, the total time is 20ms.

When P0/P2 is assigned to CPU1 while P1 is assigned to CPU2, the total time is 25ms.

When P1/P2 is assigned to CPU1 while P0 is assigned to CPU2, the total time is 30ms.

When all the programs are assigned to one CPU, the total time is 35ms.

## Ex4

Solution 1: $25*80*8/8(Byte/bit)/1024(KB/Byte)=1.953125KB$

Solution 2: $1024*768*24/8(Byte/bit)/1024(KB/Byte)=2304KB$

Price in 1980es: \$9.77, \$11520 correspondingly.

Price today: \$10/GB, so that the price is much lower in these days.

## Ex5. Command lines on a Unix system

```shell
# VE482 h1 ex5
# Create a new user
user add -m william
# List all the currently running processes
ps -alE
# Display the characteristics of the CPU and the available memory
cat /proc/cpuinfo
cat /proc/meminfo
# Redirect some random output into two different files
head -n 1 /dev/random | tee file1.txt > file2.txt
# Concatenate the two previous files
cat file1.txt file2.txt > file3.txt
# Read the content of the resulting file as hexdecimal values (in other words find a command to read a file as hexadecimal values)
hexdump file1.txt
# Use a single command to find all the files in /usr/src with the word semaphore in their name and containing the word ddekit_sem_down
grep -Rl "ddekit_sem_down" /usr/src | grep "semaphore"

```

*Note: the code is saved in the file `ex5.sh`.* [^3]



> check for correctness

[^1]: “What happens when we turn on computer? - GeeksforGeeks,” *GeeksforGeeks*, 08-Apr-2017. [Online]. Available: https://www.geeksforgeeks.org/what-happens-when-we-turn-on-computer/. [Accessed: 20-Sep-2020].
[^2 ]: Vithusha Aarabhi, “Exokernels : An Operating System Architecture for Application Level Resource Management,” *Medium*, 02-Sep-2017. [Online]. Available: https://medium.com/@vithushaaarabhi/exokernels-an-operating-system-architecture-for-application-level-resource-management-32d0daaeeab0. [Accessed: 20-Sep-2020].
[^3]: “Minix Man Pages: Index Page,” *Minix3.org*, 2020. [Online]. Available: https://man.minix3.org/cgi-bin/man.cgi?manpath=Minix+3.1.5. [Accessed: 20-Sep-2020].



‌

‌
# VE482-public
*A collection of materials and self-written solutions for the course Introduction to Operating System (VE482-FA20) in UMJI, taught by prof. Manuel Charlemagne.*

## Overview
### Lab
| Index | Major Focus | Lab Design | Story |
| ----- | ----------- | ---------- | ----- |
| 01    | hardware overview, Unix command, shell feature | basic shell script | - |
| 02    | Minix3 overview, ssh, basic git | Minix3 setup | - |
| 03    | practice git, useful linux commands, sed, awd | shell web crawler | - |
| 04    | database, gdb | database query | O |
| 05    | layer programming, libraries | practice layer programming | - |
| 06    | plugin structure | open-source plugin software contribution | - |
| 07    | linux kernel source code, kernel module | dadfs (file system code alternation) | O |
| 08    | virtual memory, page-replacement algorithm, Minix3 memory management | Minix3 MRU algorithm implementation | O |
| 09    | linux kernel module programming, char device driver, user/kernel communication | dice (kernel char device) | O |
| 10    | linux kernel module programming, basic security, linux system management | hacking scripts | O |
| 11    | filesystem, userspace fs programming, FUSE | lemondb alternation | O |

### Homework
| Index | Major Focus |  MISC Practice | MISC Research  |
| ----- | ----------- | ---- | ---- |
| 1 | hardware overview | Unix shell basics| - |
| 2 | multiprogramming | Minix 3 OS basics (p3) | - |
| 3 | process and threads | C function pointer | POSIX standard |
| 4 | interprocess communication | race condition demo, semaphore implementation | - |
| 5 | scheduling, deadlock | banker's algorithm | Minix 3 scheduling (p3) |
| 6 | memory management, security | thrashing demo | Meltdown & Spectre, Minix 3 memory management (p3), Dirty COW bug |
| 7 | memory management, filesystem | Minix 3 system call (p3) | ext2 filesystem |
| 8 | IO, filesystem, security, multiprocessor | kernel panic demo | linux firewall |

### Project 
- p1: shell implementation (`mumsh`)
    - style: solo-project
    - codebase: null
    - evaluation: JOJ test required for correctness
    - coverage: c-programing, string parsing, shell features, interprocess communication, IO, etc.
- p2: multi-threading database implementation (`Lemonion::lemonDB`)
    - style: 2+2 team-project
    - codebase: provided code template
    - evaluation: JOJ test required for correctness and efficiency
    - coverage: cpp-programming, string parsing, multi-threading, etc.
- p3: Minix3 scheduler implementation (`CoolZone::Minix3`)
    - style: 2+2 team-project
    - codebase: provided Minix3 source code
    - evaluation: project demonstration required for functionality and performance
    - coverage: c-kernel-programming, os debugging, scheduling, etc.

## Honor Code
If there is similar course materials assigned in the future, it is the responsibility of JI students not to copy or modify these codes, or MD/PDF files because it is against the Honor Code. The owner of this repository doesn't take any commitment for other's faults.

## Contribution
Help yourself to [raise any issues](https://github.com/MatrixPecker/VE482-public/issues) if you find any mistake in the solution.

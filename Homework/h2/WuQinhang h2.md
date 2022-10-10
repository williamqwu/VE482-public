# VE482 Homework 2

<center><div style="width:100%;height:57%;text-align:center;font-size:16px;line-height:25px;font-family: 'Courier New', Courier, monospace;font-weight:300;"><b>Due: Oct.4<br>Name: Wu Qinhang<br>ID: 518370910041<br>Email: william_wu@sjtu.edu.cn</b>
</div></center>

## Multiprogramming

### 1.1

**Probability**: $p^n$

CPU utilization: $1-p^n$

### 1.2

![image-20201002215645041](C:\Users\William Wu\Documents\Mainframe Files\UMJI-SJTU\1 Academy\20 Fall\VE482\HW-related\h2\WuQinhang h2.assets\image-20201002215645041.png)

### 1.3

a) **Three**.

b) By the equation deduced in 1.1, the CPU utilization is **0.271**.

c) 

- adding 256MB: the CPU utilization is **0.570**. Increase: 0.117%/MB
- adding 512MB: the CPU utilization is **0.771**. Increase: 0.098%/MB
- adding 1024MB: the CPU utilization is **0.920**. Increase: 0.063%/MB

By comparison of the calculation result, we see that adding another **256MB** will be the best investment.

## Minix 3 Keymap[^1]

in `/usr/src/servers/is/dmp.c`, struct hook_entry, instant hooks, added:[^2]

```c
{SF7, proccnt_dmp, "Running process conut"},
```

in `/usr/src/servers/is/dmp_kernel.c`, added:[^3]

```c
#include "../pm/mproc.h"

/* proccnt_dmp */
void proccnt_dmp(){
	struct mproc *pc;
  int cnt = 0;
  if (getsysinfo(PM_PROC_NR, SI_PROC_TAB, mproc, sizeof(mproc)) != OK){
    printf("Get Process Count Error.\n");
    return;
  }
  int i;
  for(i=0;i<NR_PROCS;i++){ // mproc[NR_PROCS]
    pc=&mproc[i];
    if(pc->mp_pid==0 && i!=PM_PROC_NR) continue; // Signaled by PM, ignore
    cnt++;
  }
  printf("Current number of running processes: %d\n",cnt);
}
```

in `/usr/src/servers/is/proto.h`, added:

```c
void proccnt_dmp(void);
```

```shell
cd /usr/src
make build
reboot
```

**Recompile test result**:

![image-20201003123148830](C:\Users\William Wu\Documents\Mainframe Files\UMJI-SJTU\1 Academy\20 Fall\VE482\HW-related\h2\WuQinhang h2.assets\image-20201003123148830.png)

![image-20201003123158530](C:\Users\William Wu\Documents\Mainframe Files\UMJI-SJTU\1 Academy\20 Fall\VE482\HW-related\h2\WuQinhang h2.assets\image-20201003123158530.png)

[^1]:skr, “F4 function key in minix 3.1.3,” *Stack Overflow*, 29-Jan-2016. [Online]. Available: https://stackoverflow.com/questions/35092125/f4-function-key-in-minix-3-1-3. [Accessed: 03-Oct-2020].
[^2]:"How to Use the vi Editor," *Washington.edu*, 2020. [Online]. Available: https://www.washington.edu/computing/unix/vi.html#:~:text=vi%20can%20be%20used%20from,shows%2024%20lines%20of%20text. [Accessed: 03-Oct-2020].

[^3]:A. Jerrett, “Number of Running Processes on a Minix system from C code,” *Stack Overflow*, 29-Aug-2011. [Online]. Available: https://stackoverflow.com/questions/7234369/number-of-running-processes-on-a-minix-system-from-c-code. [Accessed: 03-Oct-2020].
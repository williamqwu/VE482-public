<div style="width:60%;height:200px;text-align:center;border:14px solid #808080;border-top:none;border-left:none;border-bottom:none;display:inline-block">
    <div style="border:4px solid #808080;border-radius:8px;width:95%;height:100%;background-color: rgb(209, 209, 209);">
        <div style="width:100%;height:30%;text-align:center;line-height:60px;font-size:26px;font-family:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;">VE482 Lab Report</div>
        <div style="width:100%;height:18%;text-align:center;line-height:26px;font-size:20px;font-familny:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;"><b>Lab 8</b> - Fall 2020</div>
        <div style="width:100%;height:57%;text-align:center;font-size:16px;line-height:22px;font-family: 'Courier New', Courier, monospace;font-weight:300;"><br><b>Wu Qinhang (518370910041)<br>Sun Zhimin (518370910219)<br></b></div>
    </div>
</div>
<div style="width:35%;height:200px;display:inline-block;float:right">
    <div style="width:100%;height:25%;text-align:center;line-height:55px;font-size:20px;font-family:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;"><b>Table of Contents</b></div>
    <div style="width:100%;height:75%;text-align:left;margin-left:2px;line-height:30px;font-size:13px;font-family:Verdana, Geneva, Tahoma, sans-serif;font-weight:300;">• Virtual Memory<br>• Page Replace Algorithm<br>• Minix 3 Memory Management</div>
</div>



[TOC]
## Memory Management at Kernel Level
**What does `vm` stands for?**
- ~~VMware~~
- Virtual Memory

**Find all the places where the vm used inside the kernel, Why does it appear in so many different places?**[^3]

- `find /usr/src -name "*.c" | xargs grep -l "vm" > vm_data`
- ![img](https://notes.sjtu.edu.cn/uploads/upload_46600de7af7e88c8f1b9e8abc0a6685e.png)
- I find 300 source files altogether that contain virtual memory, a very large number.
- Virtual memory is always being used, because all the process require its only virtual process, and even when the memory that is required by all running processes does not exceed the volume of RAM that is installed on the system, vm is still used.

**How is memory allocated within the kernel? Why are not malloc and calloc used?** [^4]

- Instead of `malloc` or `calloc`, in the kernel, many new functions are used:

  ```c
  #include <linux/slab.h>
  
  void *kmalloc(size_t size, int flags);
  // kmalloc(): get any size of block
  void *vmalloc(size_t size);
  // vmalloc(): allocate contiguous virtual memory
  ```
  It's quite similar to `malloc`, except that it allocates a block of **kernel memory** (or **virtual memory**).

- Since `malloc` and `calloc` are user library functions, the memory that allocated by those two functions is also accessible to users, and that results in the issue of kernel's security.

- IPC (Message passing) is used to send message between kernel space and user space. There is a structure defined in `/include/minix/ipc.h`:
```c
typedef struct {
  endpoint_t m_source;		/* who sent the message */
  int m_type;			/* what kind of message is it */
  union {
	mess_1 m_m1;
	mess_2 m_m2;
	mess_3 m_m3;
	mess_4 m_m4;
	mess_5 m_m5;
	mess_7 m_m7;
	mess_8 m_m8;
	mess_6 m_m6;
	mess_9 m_m9;
	mess_10 m_m10;
  } m_u;
} message __aligned(16);
```

**While allocating memory, how does the functions in kernel space switch back and forth between user and kernel spaces? How is that boundary crossed? How good or bad it is to put vm in userspace?**
- When the user calls the function, e.g. `malloc` or `calloc`, take Minix3 as an example, the kernel calls the following functions one by one:

  1. `mmap()`
  2. `do_mmap()`
  3. `map_page_region()`
  4. `map_ph_writept()`
  5. `pt_writemap()`

  General idea is to ask the memory management unit for a block of memory, next establish a map connection of that block, then write that part onto the page table, finally return the pointer to the userspace, who can use it to have access to the memory block.

- By mapping user-space memory into the kernel (with get_user_pages).
- It depends. It's good for our usual PC, since it can prevent the user from changing the memory intentionally or accidently, thus protecting our computer. However, for real-time system or embedded system, the kernel is designed to serve, and two separate space (userspace and kernel-space are unnecessary).

**How are pagefaults handled?**
- When a pagefault occurred, the operating system first search to see if there is available page frame in the physical memory. If so, it set up a mapping relationship between the page and the page frame. If not, it locate a page frame in the physical memory that is not frequently used and write its content onto the hard drive. After that, it reads the page onto the page frame and set up a mapping relationship between them and redo the instruction.

## MRU Implementation and Analysis [^1]
**What algorithm is used by default in Minix 3 to handle pagefault? Find its implementation and study it closely.**

- Minix 3.2.1 is using LRU algorithm to handle pagefault. The detailed implementation is in `/servers/vm/pagefault.c`.
    - `char *pf_errstr(u32_t err)` is an error handling function.
    - `void do_pagefaults(message *m)` is the page fault handler.
    - `void do_memory(void)`: when `SIGKMEM` (kernel memory pending request) is asserted, `handle_memory` will be called to allocate the memory.
    - `int handle_memory(struct vmproc *vmp, vir_bytes mem, vir_bytes len, int wrflag)` call `map_handle_memory` to allocate the memory according to the page size.

**Use the top command to keep track of your used memory and cache, then run `time grep -r "mum" /usr/src`. Run the command again. What do you notice?** [^2]
- This command will search for the pattern "mum" in Minix 3.2.1 source code.
- The first trial: `8.83 real / 0.28 user / 5.81 sys`
- The second trial: `3.03 real / 0.88 user / 2.08 sys`
- The third trial: `3.11 real / 0.95 user / 2.16 sys`
- We can conclude that calculation relevant with the searching process in the first trial is cached and perserved for a certain amount of time, so that in a neighbouring search process, the result is returned about two times faster. It matches LRU algorithm. 

**Adjust the implementation of LRU into MRU and recompile the kernel.**

Change:

- in file `/lib/libminixfs/cache.c/lmfs_get_block(dev_t,block_t,int)`, the lower level implementation of LRU is changed into MRU
- in `/servers/vm/region.c/free_yielded(vir_bytes)`, the upper level implementation of LRU (freeing node) is changed into MRU
- for source files, see attachment.

Rebuild the kernel:
```shell
# use SCP to transfer files
scp -P 2222 ./minix-R3.2.1/servers/vm/region.c root@192.168.227.129:/usr/src/servers/vm/region.c
scp -P 2222 ./minix-R3.2.1/lib/libminixfs/cache.c  root@192.168.227.129:/usr/src/lib/libminixfs/cache.c

# in Minix 3, rebuild the system
su
cd /usr/src
make build
reboot
```

Result:

![image-20201122003625766](Lab8 Report.assets/image-20201122003625766.png)

**Use the top command to keep track of your used memory and cache, then run `time grep -r "mum" /usr/src`. Run the command again. What do you notice?**

![image-20201122003640528](Lab8 Report.assets/image-20201122003640528.png)

![image-20201122003646133](Lab8 Report.assets/image-20201122003646133.png)

![image-20201122003650958](Lab8 Report.assets/image-20201122003650958.png)

- I see that the first trial runs about one time slower compared with the LRU implementation. The second trial only speed up about 25% than the first trial. It matches MRU algorithm.

**Discuss the different behaviours of LRU and MRU as well as the consequences for the users. Can you think of any situation where MRU would be better than LRU?**
- As far as page replacement algorithm is concerned, LRU will replace the page that is least recently used, while MRU will replace the page that is most recently used. In the situation when a repeated sequence of pages are continuously inquired, MRU will perform better than LRU since the most recently used page will be inquired after a long time.

**(EXTRA) How do LRU and MRU compare when many processes allocate much static and dynamic memory, and then recall it?**
- In this situation, MRU will probably perform better than LRU algorithm. Since many processes are allocating memory one by one, the most recently used page will not be retrieved in the near future, while some least recently used page may be taken advantaged of.


[^1]: Weatherly, C. (2009). MinixVM: An Implementation of Virtual Memory in Minix 3 (Doctoral dissertation, College of William & Mary).
[^2]: "What do ‘real’, ‘user’ and ‘sys’ mean in the output of time(1)?," Stack Overflow, Feb. 17, 2009. https://stackoverflow.com/questions/556405/what-do-real-user-and-sys-mean-in-the-output-of-time1 (accessed Nov. 20, 2020).
[^3]: Minix 3 Virtual Address, https://condor.depaul.edu/~glancast/443class/docs/slides/Oct06/slide16.html (accessed Nov. 20, 2020).

[^4]: kingkai, “Why malloc+memset is slower than calloc?,” Stack Overflow, Apr. 22, 2010. https://stackoverflow.com/questions/2688466/why-mallocmemset-is-slower-than-calloc (accessed Nov. 22, 2020).
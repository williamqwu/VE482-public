<div style="width:60%;height:200px;text-align:center;border:14px solid #808080;border-top:none;border-left:none;border-bottom:none;display:inline-block">
    <div style="border:4px solid #808080;border-radius:8px;width:95%;height:100%;background-color: rgb(209, 209, 209);">
        <div style="width:100%;height:30%;text-align:center;line-height:60px;font-size:26px;font-family:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;">VE482 Lab Report</div>
        <div style="width:100%;height:18%;text-align:center;line-height:26px;font-size:20px;font-familny:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;"><b>Lab 1</b> - Fall 2020</div>
        <div style="width:100%;height:57%;text-align:center;font-size:16px;line-height:22px;font-family: 'Courier New', Courier, monospace;font-weight:300;"><br><b>Name: Wu Qinhang<br>ID: 518370910041<br>Email: william_wu@sjtu.edu.cn<br></b></div>
    </div>
</div>
<div style="width:35%;height:200px;display:inline-block;float:right">
    <div style="width:100%;height:25%;text-align:center;line-height:55px;font-size:20px;font-family:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;"><b>Table of Contents</b></div>
    <div style="width:100%;height:75%;text-align:left;margin-left:2px;line-height:30px;font-size:13px;font-family:Verdana, Geneva, Tahoma, sans-serif;font-weight:300;">• Hardware Overview<br>• Basic shell<br>• Linux Filesystem Hierarchy</div>
</div>




[TOC]

## Hardware overview

### Figures

<center>
<img style="height:300px" src="img\motherboard.jpg" alt="motherboard">
<br>Figure 1. The motherboard<br><br>
<img style="height:300px" src="img\PCPowerSupply.jpg" alt="PCPowerSupply">
<br>Figure 2. The PC power supply<br><br>
    <img style="height:300px" src="img\hardDiskDrive.jpg" alt="hardDiskDrive">
<br>Figure 3. A Hard Disk Drive<br><br>
    <img style="height:300px" src="img\PCIslot.jpg" alt="PCIslot">
<br>Figure 4. A PCI slot (*Note: the PCI card is missing)<br><br>
    <img style="height:300px" src="img\opticalDiskDrive.jpg" alt="opticalDiskDrive">
<br>Figure 5. An Optical disk drive<br><br>
    <img style="height:300px" src="img\RAM.jpg" alt="RAM">
<br>Figure 6. The RAM<br><br>
    <img style="height:300px" src="img\northWithSouthBridge.jpg" alt="northWithSouthBridge">
<br>Figure 7. The North and South bridges<br><br>
    <img style="height:300px" src="img\SATASocket.jpg" alt="SATASocket">
<br>Figure 8. A SATA socket<br><br>
    <img style="height:300px" src="img\battery.jpg" alt="battery">
<br>Figure 9. The battery<br><br>
    <img style="height:300px" src="img\CPU.png" alt="CPU">
<br>Figure 10. The CPU<br><br>
    <img style="height:300px" src="img\BIOS.jpg" alt="BIOS">
<br>Figure 11. The BIOS<br><br>
</center>

### Q&A

- **Where is the CPU hidden, and why?**
  - The CPU is hidden (usually under a fan) in a structure shown in Figure 10(b). The reason is to protect the CPU, ensure a strong connection between CPU and other units, and cool down the CPU since the CPU may generate a lot of heat.
  
- **What are the North and South bridges?**
  
  - North bridge is a chip on the motherboard that manages the communication between CPU and certain parts of the motherboard. It also connects the South bridge and the CPU. South bridge is a chip that controls the IO functioning. Their relative location is described in Figure 7.
  
- **How are the North and South bridges connected together?**
  
  - They are connected by an internal bus.
  
- **What is the BIOS?**
  
  - BIOS stands for "Basic Input/Output System", which is a program that initializes the computer and manages the data flow between OS and other hardware.
  
- **Take out the CPU, rotate it and try to plug it back in a different position, is that working?**
  - No. It cannot be inserted with a different position.
  
- **Explain what overclocking is?**
  - Overclocking is the mechanism of increasing the clock rate of the computer so as to increase the operating speed.
  
- **What are pins on a PCI/PCI-e card and what are they used for?**
  - The pin on the PCI card is an electrical connection used to connect two devices electrically. There are many pins on a single PCI card for different purposes, such as testing, ground connection, VDC connection, signal transmission and so on. [^1]
  
- **Before PCI-e became a common standard many graphics cards were using Accelerated Graphics Port (AGP), explain why.**
  - AGP is twice the bandwidth of PCI, and it can assist 3D computer graphics acceleration.[^2]

## Git

*ignored for now.*

## Command line interface

### mkdir, touch, mv, cp, ls

```shell
# Create a file named test
touch test
# Move test to dir/test.txt, where dir is a new directory.
mkdir dir
mv test dir/test.txt
# Copy dir/test.txt to dir/test_copy.txt.
cp dir/test.txt dir/test_copy.txt
# List all the files contained in dir.
ls dir -a
```

### grep[^3]

```shell
# List all the files form /etc containing the pattern 127.0.0.1.
grep -rl "127.0.0.1" /etc
# Only print the lines containing your username and root in the file /etc/passwd (only one grep should be used)
# Note: my username is "william"
grep -rE "(william)|(root)" /etc/passwd
```

### find

```shell
# List all the files from /etc that have been accessed less than 24 hours ago.
find /etc -atime -1
# List all the files from /etc whose name contains the pattern “netw”.
find /etc -name "*netw*"
```

### redirection

- `>`: **Redirecting Output**, redirecting the standard output into a file (whose name is specified on the right of `>`) and overriding its original content
- `>>`: **Appending Redirected Output**, redirecting the standard output into a file (whose name is specified on the right of `>`) and appending the result after its original content
- `<<<`: **Here Strings**, passing the strings on the right of `<<<` as the standard input of the command on the left.
- `>&1`: duplicating the file descriptor on the left to be a copy of file descriptor 1 (i.e., standard output)
- `2>&1 >`: only redirecting the standard output to the file[^4]
- `tee`: redirecting the standard input to a file and print it to standard output

### xargs

- `xargs` builds and executes command lines from standard input. 
- The sign `|` embedded commands within a pipe.

### head, tail

- `head`: output the first part of files
- `tail`: output the last part of files
- We can use `tail -f` to *live display* a file as new lines are appended.

### system monitoring

- `ps`: report a snapshot of the current processes
- `top`: provide a dynamic real-time view of the system
- `free`: display the amount of free and used memory in the system
- `vmstat`: report virtual memory statistics, including processes, memory, paging, block IO, traps, disks, and cpu activity

### shell difference

- `sh`: original shell
- `bash`: official shell on most Linux system
- `csh`: shell written by C
- `zsh`: more useful features compared with bash

### `$`

- `$0`: the first argument to the script
- `$1`: the second argument to the script...
- `$?`: the exit code from the previous command
- `$!`: the process ID of the most recently executed background pipeline [^5]

### PS3

`PS3` is an environment variable in bash that stands for the prompt message inside a shell script. It can be modified so as to provide a more user-friendly prompt.

```shell
# code example for PS3
PS3='Select a number (1-3): '
select choice in 'Apple' 'Banana' 'Cabbage'
do
    echo -ne "Today's breakfast is "
    case $choice in
        Apple) echo apple ;;
        Banana) echo banana;;
        Cabbage) echo cabbage;;
        *) echo nothing;;
    esac
    break
done
```

In this way, the bash will prompt "Select a number (1-3): " instead of `#? `.

### iconv

`iconv` is used to convert text between different encodings. Usually we will have files of different encoding, especially when handling tasks on different platforms. In this case, iconv provides us with a useful tool that can handle different encoding requirements.

### `$temp`

- `${#temp}`: display the number of characters in variable `temp`
- `${temp%%word}` display the content of `temp` excluding the longest matching pattern described by `word` [^6]
- `${temp/pattern/string}`: display the content of `temp` where the pattern given by `pattern` is replaced by the pattern given by `string`

### Filesystem Hierarchy[^7]

- `/` the root directory
- `/bin` essential user binary files for system programs and utilities
- `/boot` contains the files that is essential for booting the system
- `/etc` system configuration files 
- `/lib` essential system libraries needed by the binary files in `/bin`
- `/media` when new media devices are inserted into the computer, a corresponding directory will be created here with corresponding files in the media
- `/mnt` temporary mounted files. For example, when opening wsl in win10, a folder `c` will appear under `/mnt` where every file and folder in the windows system is accessible.
- `/usr/bin` user binary files for user applications
- `/usr/share` contains architecture-independent shareable text files
- `/usr/lib` contains user libraries needed by the binary files in `/usr/bin`
- `/usr/src` contains system-related source files
- `/proc` special system files related with process and kernel
- `/sys` an interface to the kernel that provides kernel-view information and configuration settings 
- `/srv` contains data for services provided by the system
- `/opt` contains subdirectories for optional software packages
- `/var` contains variable data, typically system logging files.
- `/sbin` usually contains files for system maintenance or administrative tasks
- `/dev` contains device files
- `/vmlinuz` linux kernel executable
- `/initrd.img` a link to the latest installed initrd (i.e., Linux initial RAM disk)

## Game

```shell
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
```

*the shell program can be found in game.sh*

## Reference

[^1]: “PCI · AllPinouts,” *Allpinouts.org*, Mar-1998. [Online]. Available: https://allpinouts.org/pinouts/connectors/buses/pci/. [Accessed: 09-Sep-2020].

[^2]: J. Bernstein, “PCI vs. AGP vs. PCI Express Video Cards,” *Onlinecomputertips.com*, 27-Oct-2011. [Online]. Available: https://www.onlinecomputertips.com/support-categories/hardware/493-pci-vs-agp-vs-pci-express-video-cards/. [Accessed: 09-Sep-2020].

[^3]: “Regex Cheat Sheet,” *Rexegg.com*, 2020. [Online]. Available: https://www.rexegg.com/regex-quickstart.html. [Accessed: 09-Sep-2020].

[^4]: “Bash One-Liners Explained, Part III: All about redirections,” *Catonmat.net*, 2020. [Online]. Available: https://catonmat.net/bash-one-liners-explained-part-three. [Accessed: 09-Sep-2020].‌

[^5]: “Special Parameters (Bash Reference Manual),” *Gnu.org*, 2020. [Online]. Available: https://www.gnu.org/software/bash/manual/html_node/Special-Parameters.html. [Accessed: 11-Sep-2020].‌
[^6]: “Shell Parameter Expansion (Bash Reference Manual),” *Gnu.org*, 2020. [Online]. Available: https://www.gnu.org/software/bash/manual/html_node/Shell-Parameter-Expansion.html. [Accessed: 11-Sep-2020].

[^7]: “Linux Filesystem Hierarchy,” *Tldp.org*, 2020. [Online]. Available: https://tldp.org/LDP/Linux-Filesystem-Hierarchy/html/. [Accessed: 15-Sep-2020].



‌

‌
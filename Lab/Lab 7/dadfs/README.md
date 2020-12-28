|
| Dadfs 1.0 Architecture + Notes
|_______________________________

Block Zero = Super block
Block One = Inode store
Block Two = Occupied by the initial file that is created as part of the mkfs.

Only a limited number of filesystem objects are supported.
Files and directories can be created. Support for .create and .mkdir is implemented. Nested directories can be created.

A file cannot grow beyond one block. ENOSPC will be returned as an error on attempting to do.

Directories store the children inode number and name in their data blocks.

Read support is implemented.

Basic write support is implemented. Writes may not succeed if done in an offset. Works when you overwrite the entire block.

Locks are not well thought-out. The current locking scheme works but needs more analysis + code reviews.

Memory leaks may (will ?) exist.

|
| Credits
|________

Minor modifications by dad himself.

The initial source code is written by psankar and patches are contributed by other github members. azat being the first contributor.

An O_LARGETHANKS to the guidance of VijaiBabu M and Santosh Venugopal. Their excellent talks on filesystems motivated me to implement a file system from the scratch. Without their inspirational speeches, I would not have focussed on filesystems.

A big thanks should go to the kernelnewbies mailing list for helping me with clarifying the nitty-gritties of the linux kernel, especially people like Mulyadi Santosa, Valdis Kletnieks, Manish Katiyar, Rajat Sharma etc.

Special thanks to Ankit Jain who provides interesting conversations to help my intellectual curiosities.

Original source code before dad's modifications: https://github.com/psankar/simplefs

|
| Compilation
|____________

Install linux kernel sources and run make from the checkedout directory.

|
| Testing
|________

Simple process to test dadfs features:

0. install dependencies

1. compile: make

2. create a small virtual disk (to be formated in dadfs): dd bs=4096 count=100 if=/dev/zero of=disk

3. create a small virtual disk (to be used as dadfs' journal): dd bs=1M count=10 if=/dev/zero of=journal

4. initialise the journla: mke2fs -b 4096 -O journal_dev journal

5. format the disk: ./mkfs-dadfs disk

6. load dadfs module: insmod

7. mount disk: losetup, mount (loop,journal_path)

8. play with dad filesystem: mkdir, mv, cp, cat, rm, ls, cd, touch, etc.

9. check the logs: /var/log, dmesg

10. umount disk: losetup, umount

11. unload module: rmmod

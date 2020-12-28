#!/bin/bash
# use SCP to transfer files
scp -P 2222 ./minix-R3.2.1/servers/vm/region.c root@192.168.227.129:/usr/src/servers/vm/region.c
scp -P 2222 ./minix-R3.2.1/lib/libminixfs/cache.c  root@192.168.227.129:/usr/src/lib/libminixfs/cache.c

# alternative: mount
mkdir mountpoint
sshfs root@192.168.X.X:/usr/src mountpoint

# in Minix 3, rebuild the system
su
cd /usr/src
make build
reboot

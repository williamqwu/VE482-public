obj-m := dicedevice.o
dicedevice-objs := dice.o

all: ko

ko:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

transf:
	scp -rv ./dice william@192.168.1.111:/home/william/code/ve482-l9/dice

load:
	insmod ./dicedevice.ko
	cat /proc/modules | grep "dicedevice"
	cat /proc/devices | grep "Dice"

reg240:
	mknod /dev/dice0 c 240 0
	mknod /dev/dice1 c 240 1
	mknod /dev/dice2 c 240 2

rmdev:
	rm -f /dev/dice0
	rm -f /dev/dice1
	rm -f /dev/dice2
	rmmod dicedevice.ko
	
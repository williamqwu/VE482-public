# insert module
sudo insmod ./dicedevice.ko
# display drivers and devices
cat /proc/modules | grep "dicedevice"
cat /proc/devices | grep "Dice"
# creating device files
sudo mknod /dev/dice0 c 240 0
sudo mknod /dev/dice1 c 240 1
sudo mknod /dev/dice2 c 240 2
# testing
cat /dev/dice1
echo 1 > /dev/dice1

# remove device files
sudo rm -f /dev/dice0
sudo rm -f /dev/dice1
sudo rm -f /dev/dice2
# remove module
sudo rmmod dicedevice.ko

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

# Presentation Topics



```
passwd
pkgin undate
pkgin install openssh
vi /usr/pkg/etc/ssh/sshd_config
#Port 22 -> Port 2222
useradd -m xxx
```



## Presentation Topics

According to the milestones and your progress on the project, we divide the topics into 2 sections. The first section will be presented in lab 2 (Sept. 15) and the second in lab 3 (Sept. 22).

### Lab 2 Topics - To get you started with `mumsh`

1. what are system calls and why we need system calls for shell

2. what are parent/child processes, and usage of `fork()` and `wait()`/`waitpid()`

3. how to execute a simple command, such as `ls`, both with/without argument (`execvp()`)

4. bash style redirection syntax, e.g.

   ```
   <1.txt>3.txt cat 2.txt 4.txt
   ```

5. quotation marks in shell scripts, e.g.

   ```
   echo "de'f' ghi" '123"a"bc' a b c
   "echo" "<
   1.'txt'"
   ```

6. bash pipeline syntax, e.g.

   ```
   echo 123 | grep 1
   ```

   and implementing pipeline (`pipe()`)

7. implementing redirection (`dup2()`/`dup()`)

8. opening (`open()`) and closing (`close()`) files, what is the connection between file permission and `mode` in the parameter of `open()`?

### Lab 3 Topics - to help you organize your project better

1. how to organize the read/parse/execute loop (how to have a better structure organization of the shell code, e.g. for further extension/modification)

2. how to parse input (consider all cases including without arguments, with arguments and redirection)

3. problems likely to lead to memory leak and how to prevent/solve

4. implementation of built-in commands

5. signals and signal handler (`signal()`, handling `CTRL`+`C`)

6. what are background processes and their output format, such as

   ```
   ls &
   ```

7. identification of possible errors, such as failing to fork and

   ```
   echo abc > | grep abc # syntax errors
   echooo b # unable to find command
   ```

   and error handling (`perror()` or `printf()`).

## Topic Selection

Each group can select 1 topic from the above 15 topics. The valid period is between NOW to 12 am on September 15th.

### Format

Group Number / Lab Number / Topic Index

E.g.

1 / 2 / 2

3 / 3 / 7

You only need to list one topic you're interested in. **Please comment below to register**.

### Rule

You will receive "Pass"/"Fail" comments (You may "fail" if your topic is already chosen or your reply has the wrong format). Reply **directly** to our comment if you want to change your decision. We follow the rule of First Come, First Serve. We will randomly assign those who don't choose.
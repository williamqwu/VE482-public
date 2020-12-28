<div style="width:60%;height:200px;text-align:center;border:14px solid #808080;border-top:none;border-left:none;border-bottom:none;display:inline-block">
    <div style="border:4px solid #808080;border-radius:8px;width:95%;height:100%;background-color: rgb(209, 209, 209);">
        <div style="width:100%;height:30%;text-align:center;line-height:60px;font-size:26px;font-family:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;">VE482 Lab Report</div>
        <div style="width:100%;height:18%;text-align:center;line-height:26px;font-size:20px;font-familny:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;"><b>Lab 3</b> - Fall 2020</div>
        <div style="width:100%;height:57%;text-align:center;font-size:16px;line-height:22px;font-family: 'Courier New', Courier, monospace;font-weight:300;"><br><b>Name: Wu Qinhang<br>ID: 518370910041<br>Email: william_wu@sjtu.edu.cn<br></b></div>
    </div>
</div>
<div style="width:35%;height:200px;display:inline-block;float:right">
    <div style="width:100%;height:25%;text-align:center;line-height:55px;font-size:20px;font-family:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;"><b>Table of Contents</b></div>
    <div style="width:100%;height:75%;text-align:left;margin-left:2px;line-height:30px;font-size:13px;font-family:Verdana, Geneva, Tahoma, sans-serif;font-weight:300;">• Git in practice<br>• useful linux commands<br>• sed/awk<br>• regular expression</div>
</div>



[TOC]

## Git in practice

*Git branch game accessed from [this site](https://learngitbranching.js.org/?locale=en_US).*

### Introduction Sequence

```shell
# 1.1 Git Commit
git commit
git commit
```

```shell
# 1.2 Git Branches: branch early, and branch often
git checkout -b bugFix
```

```shell
# 1.3 Merging in Git
git checkout -b bugFix
git commit
git checkout master
git commit
# Merge branch bugFix into master
git merge bugFix
```

```shell
# 1.4 Rebase Introduction: making a nice linear sequence of commits
git checkout -b bugFix
git commit
git checkout master
git commit
git checkout bugFix
git bebase master
```

### Ramping Up

```shell
# 2.1 Moving around in Git
# detach HEAD from the branch
git checkout C4
```

```shell
# 2.2 Relative Refs
git checkout bugFix^
```

```shell
# 2.3 ~ operator
git checkout HEAD^
# move (by force) the branch to ...
git branch -f bugFix C0
git branch -f master C6
```

```shell
# 2.4 Reversing Changes in Git
# revsese the most recent commit on local and pushed
git reset HEAD~1
git checkout pushed
git revert HEAD
```

### A mixed bag

```shell
# 4.1 Locally stacked commits
git checkout master
git cherry-pick bugFix
```

```shell
# 4.4 Git Tag
git tag v0 c1
git tag v1 C2
git checkout C2
```

### Remote

```shell
# 6.1 Git Remotes
git clone
```

```shell
# 6.2 Git Remote Branches
git commit
git checkout o/master
git commit
```

```shell
# 6.3 Git Fetch
git fetch
```

```shell
# 6.4 Git Pill: fetch and merge
git pull
```

```shell
# 6.6 Git Push
git commit
git commit
git push
```

![image-20201002162724556](C:\Users\William Wu\Documents\Mainframe Files\UMJI-SJTU\1 Academy\20 Fall\VE482\Lab\Lab 3\WuQinhang Lab3 Report.assets\image-20201002162724556.png)![image-20201002162733229](C:\Users\William Wu\Documents\Mainframe Files\UMJI-SJTU\1 Academy\20 Fall\VE482\Lab\Lab 3\WuQinhang Lab3 Report.assets\image-20201002162733229.png)

## Source code

### `rsync`

```shell
# rsync in Minix 3
pkgin install rsync
man rsync
# Create an exact copy of the directory /usr/src into the directory /usr/src_orig
cp -r /usr/src /usr/src_orig
# Create an exact copy of the Minix 3 directory /usr/src_orig into your Linux system, using rsync and ssh
# note: -e "ssh -p $(PortNumber)" is used to choose an alternative remote shell program
rsync -avz -e "ssh -p 2222" root@192.168.227.129:/usr/src_orig ~/qhhtmp
```

### `diff`/`patch`

```shell
man diff
man patch
# create a patch corresponding to my changes
diff -Naur /usr/src_orig /usr/src > patch.txt
# apply a patch
patch -p3 < patch.txt
# revert a patch
patch -R < patch.txt
```

## Scripting

### air quality info[^1][^2][^3]

```shell
curl "https://aqicn.org/?city=Shanghai&widgetscript&size=large&id=52b39d71decf07.20261781" 2> /dev/null | sed -rn 's/.*title=\\.*\">([[:digit:]]{1,3}).*hdrt.*10px.*>([-]?[[:digit:]]{1,5})<.*/AQ: \1 Temp: \2 ºC/p'
```

*data set accessed from [this site](https://aqicn.org/?city=Shanghai&widgetscript&size=large&id=52b39d71decf07.20261781).*

### ip address

```shell
ifconfig | awk -e '{for(i=1;i<=NF;i++){ if($i=="inet"){print $(i+1)} } }'
```

[^1]: "Regex101 - online regex editor and debugger,"*@regex101*, 2020. [Online]. Available: https://regex101.com/. [Accessed: 29-Sep-2020].
[^2]: Pablo, "How to output only captured groups with sed?,"*Stack Overflow*, 06-May-2010. [Online]. Available: https://stackoverflow.com/questions/2777579/how-to-output-only-captured-groups-with-sed/43997253. [Accessed: 29-Sep-2020].
[^3]: Operator Precedence of Regular Expression, POSIX. [Online]. Available: https://www.boost.org/doc/libs/1_56_0/libs/regex/doc/html/boost_regex/syntax/basic_extended.html#boost_regex.syntax.basic_extended.operator_precedence


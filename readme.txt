The main file is om.c
The input is read and tokenized in line.c
All the commands to execute are stored in execute.c
All function definitons are stored in headers.h

Use & for bg processes
Seperate arguments by spaces or tabs and commands by ;
Use pinfo [pid] to know details of the process 
cd, pwd, echo, help, pinfo, and exit are  builtin

use > for output redirection, < for input and >> for appending to a file (make sure to use spaces eg. cat < inputfile)
| is used to pipe and can be used to pipe multiple commands
jobs will list bg processes
ctrl + D will exit the terminal
ctrl + C will exit a process in the terminal but the terminal keeps running

use ./om to start execution
cd ~ takes u to the directory from which this shell was compiled and executed(assumed as home directory)


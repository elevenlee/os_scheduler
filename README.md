os_scheduler
============

In this project we will test several scheduling algorithm.


1. The code was written under the Ubuntu Linux System (Version 11.10).
2. The Compiler version is GCC 4.6.1.
3. I have written a "makefile" document.
   So just type "make" command under current directory to compile source code.
   Type "make clean" under current directory to remove object and execution files.
4. The format of running source code is as below:

   ./Scheduler <input file name> <schedule method> [<output file name>]

   (1) The <input file name> argument is necessary;
   (2) The <schedule method> argument is also necessary;
       1: First Come First Served
       2: Round-Robin with quantum 2
       3: Shortest Remaining Job First
   (3) The <output file name> argument is optinal.
       If you do not input this argument, the output file's name would be "output.txt" automatically.

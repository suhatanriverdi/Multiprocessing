# Multiprocessing
Multiprocessing C program which includes **fork(), pipe(), read(), write(), close(), waitpid()**, and other appropriate system calls in Unix Systems. This assignment is given in Operating Systems Course.

Applications in a multiprocessing system are broken to smaller routines that run independently. In this assignment, our problem statement is that we allow the main  process to generate a text file. The main process will then create two other processes and pass to them the name of the file The codes are two different alphabetic letters such as “b” and “h”.<br><br>
•	The child processes should search the file for the character in the interval received, compute their  frequencies   and return them through a separate pipe to  the parent process. <br><br>
•	The parent process should compute the total number of characters in the file, and the rate of the appearance of the character frequencies received from the two child process, through separate pipes.<br><br>
•	The parent process should then form a table of alphabetic characters and their frequencies and print  the table in a proper format on the screen.<br>

•	In order to run this code, you need to have a Linux Distribution installed on your machine or you can also use a virtual machine. The following steps describe how to run the code assuming you are working on Linux;

**-> Firstly, Open a terminal and Navigate to the directory where your source code is saved.
For example, your source code file will be, “201611689.c”, is located in /home/yourname, type;**<br>
## cd /home/yourname <br>
->  Then type;
## gcc main.c –o new
-> Now you can run your compiled program by typing;
## ./new

-> The output should be as following screenshot;

![201611689 - SCREENSHOT](https://user-images.githubusercontent.com/36234545/58896702-383fd280-86ff-11e9-8106-6460c4ad7fdf.png)

## License
Multiprocessing C program is licensed under the MIT license. See LICENSE for more information.

## Project Status
You can download the latest release from this repository.

## Disclaimer
This project was prepared and shared for educational purposes only. You can use or edit as you wish :)

## About
Süha TANRIVERDİ Çankaya University, Computer Engineering

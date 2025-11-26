# System Programming Lab 11 Multiprocessing


## Brief overview of the Multiprocessing lab 
This lab's premise was to make a movie in the .mpg format from 50 .jpg images, created by multiple child processes. The more processes we have, the faster the file executes. This program uses "fork()" to create a child process and run the file mandel.c. 
Each of these children, which is user declared by the input-p, or uses the default if not provided, creates exactly one frame and then exits the program using the exit(1) command. The parent is in charge of actively keeping track of all the children and waiting for each of them, so we do not break the code. 

## Lab 12 Modification
This lab's premise was to make 50 JPG images using threading. I decided to use a struct to make this all pan out nicely.
Each thread use the structs argument to have its associated row and column to finish.

To compile this file, Dr. Turney provided a very convenient, neat Makefile. 



## Discussion of Runtime Results
<img width="182" height="472" alt="image" src="https://github.com/user-attachments/assets/90579c7a-0955-4e12-8b23-ae58342fb76a" />



This table clearly shows how more threads and processes affect our runtime, the more of either we have -> the quicker our runtime will be.

# System Programming Lab 12 Multithreading


## Brief overview of the Multiprocessing lab 
This lab's premise was to make a movie in the .mpg format from 50 .jpg images, created by multiple child processes. The more processes we have, the faster the file executes. This program uses "fork()" to create a child process and run the file mandel.c. 
Each of these children, which is user declared by the input-p, or uses the default if not provided, creates exactly one frame and then exits the program using the exit(1) command. The parent is in charge of actively keeping track of all the children and waiting for each of them, so we do not break the code. 

## Lab 12 Modification Overview
This lab's premise was to make 50 JPG images using threading. I decided to use a struct to make this all pan out nicely.
Each thread use the structs argument to have its associated row and column to finish.
I then wait for all the threads to finish doing what they are doing then join them with pthread_join(), to compile cleanly without any deadlocks or such.
This program can take up to 20 threads! 
Select a thread count 1 through 20, and use -t # to set and compile your count. This can also be use alongside -p.
To compile this file, Dr. Turney provided a very convenient, neat Makefile. 

## Question answers
Question 1 - which is faster? 
Answer - I believe multithreading since they all target 1 objective with their shared memory, you can't tell very cleanly from the table provided due to hardware restrictions but, I believe multiprocessing is longer since memory is not shared.
Question 2 - Sweet spot? 
Answer - There is a sweet spot actually, this lands between 10-20 threads. I believe this was because I only have 12 cores on a computer, this may change if my computer was more powerful.


## Discussion of Runtime Results
<img width="182" height="472" alt="image" src="https://github.com/user-attachments/assets/90579c7a-0955-4e12-8b23-ae58342fb76a" />



This table clearly shows how more threads and processes affect our runtime, the more of either we have -> the quicker our runtime will be.

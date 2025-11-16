/*
* Name: Ashwin Senthil
* Professor: Dr. Turney
* Date: 11/15/2025
* 
* 
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int children = 1; //this is our # of processes
    int opt;


    while((opt = getopt(argc, argv, "p:")) != -1){
        switch (opt){
            case 'p':
            children = atoi(optarg); //switch the global we got and set to the process amt #
            break;
            default:
            printf("Did not give a number\n");
            exit(1);
        }
    }

    //validity check on #
    if (children < 1){
        printf("process count needs to be positive and greater then 0\n");
        exit(1);
    }

    //check the # we got, debug 
    printf("# of processes we read: %d\n", children); //j checking our atoi

    //fork()

    //produce 50 frames
    double scale = 4.0; //default zoom

    for(int i = 0; i < 50; i++){
        
         //if created num_child, wait to finish
        if(i >= children){
            wait(NULL);
        }
        pid_t pid = fork(); //create a child node

    

        if(pid == 0){
            //child process
            printf("Child from frame %d\n", i); //child at that node
            exit (0);
        }

        //process parent
        printf("parent created process %d, from frame %d\n", pid, i);
    
    }

    //wait for all child
    while (wait(NULL)>0);
    return 0;
}
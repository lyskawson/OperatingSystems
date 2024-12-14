#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>


#define _XOPEN_SOURCE 600 



void handle_sigterm(int sig) 
{
    printf("SIGTERM received. Exit...\n");
    exit(0);
}

void handle_sigusr2(int sig) 
{
    printf("SIGUSR2 received. Continue...\n");
    return;
}


void handle_sigalrm(int sig) 
{
    printf("SIGALRM received.\n");
    
}

int main() 
{
    signal(SIGUSR1, SIG_IGN);               
    signal(SIGTERM, handle_sigterm);             
    signal(SIGUSR2, handle_sigusr2);           
    signal(SIGALRM, handle_sigalrm);   

    
    
    printf("Starting program...");
    printf("Process ID: %d\n", getpid());

    int counter = 0;
    while (1) 
    {
        counter++;
        sighold(SIGALRM);

        if (counter % 1000 == 0) 
        {
            printf("Enabling SIGALRM.\n");
            sigrelse(SIGALRM); // lock signal SIGALRM

            
           

            printf("Disabling SIGALRM.\n");
            sighold(SIGALRM); // unlock signal SIGALRM 
        }


        struct timespec remaining, request = {0, 10000000}; // 10 ms
        nanosleep(&request, &remaining);
    }

    return 0;
}
#include <stdio.h>
#include <unistd.h> //fork, pipe, close, read, write
#include <fcntl.h> //open
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{

    int pipefd[2]; //ends of pipe
    pid_t sub_pid;
    
    char buffer[1024];
    int readCount;

    // a) utworzenie potoku
    pipe(pipefd);

    // b) utworzenie popdprocesu
    sub_pid = fork();
    

    if (sub_pid == 0) //check if child process
    { 
        printf("Child process (PID: %d) is executing...\n", getpid());
        close(pipefd[1]); // close the write end of the pipe, because child only reads

        while ((readCount = read(pipefd[0], buffer, sizeof(buffer))) > 0) 
        {
            buffer[readCount] = '\0'; // null-terminate the buffer
            printf("@@@@%s####\n", buffer); 
        }

        close(pipefd[0]); // close the read end of the pipe
        exit(0);
    } 
    else // parent process
    { 
        printf("Parent process (PID: %d) is executing...\n", getpid());    
        close(pipefd[0]); // close the read end of the pipe, because parent only writes

      
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("Error with fopen");
            close(pipefd[1]);
            return 1;
        }


        while ((readCount = fread(buffer, 1, sizeof(buffer), file)) > 0) 
        {
            write(pipefd[1], buffer, readCount);
        }

        fclose(file); 
        close(pipefd[1]); // close the write end of the pipe
        wait(NULL); // wait for the child process to finish
    }

    return 0;
}
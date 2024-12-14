#include <stdio.h>
#include <unistd.h> //fork, pipe, close, read, write
#include <fcntl.h> //open
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h> //wait

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

        
        dup2(pipefd[0], STDIN_FILENO); //przekierowanie zawartości potoku na standardowe wejście 
        close(pipefd[0]); // close the read end of the pipe

     
        execlp("feh", "feh", "--reload", "1", NULL);
        perror("Error with execlp");
        exit(1); 
        
    } 
    else // Parent process
    { 
        printf("Parent process (PID: %d) is executing...\n", getpid());
        close(pipefd[0]); // close the read end of the pipe, because parent only writes

        // Open the binary image file for reading
        FILE *file = fopen(argv[1], "rb");
        if (!file) 
        {
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

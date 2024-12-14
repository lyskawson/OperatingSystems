#include <stdio.h>
#include <unistd.h> // write, close, sleep
#include <fcntl.h>  // open
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h> // mkfifo


int main(int argc, char *argv[]) 
{

   mkfifo("fifo_pipe", 0666);
   int fifo_fd = open("fifo_pipe", O_WRONLY);

    char buffer[BUFSIZ];
    int readCount;

    for (int i = 1; i < argc; i++) 
    {
        printf("Sending file: %s to FIFO\n", argv[i]);

      
        FILE *file = fopen(argv[i], "r");
        if (!file) 
        {
            printf("Canot open file: %s ", argv[i]);
            continue;
        }


        while ((readCount = fread(buffer, 1, sizeof(buffer), file)) > 0) 
        {
            write(fifo_fd, buffer, readCount);
        }

        fclose(file); 

        printf("Waiting...\n");
        sleep(5);
    }

    close(fifo_fd); 
    printf("Sending compeleted.\n");

    return 0;
}

//wyswietlanie za pomoca tail -c +1 -f fifo_pipe
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define BABBLE_NAME "barwlo_alys"
#define BABBLE_MODE 0777
#define BABBLE_LIMIT 10 // max number of messages
#define BABBLE_LENGTH 80 // max length of message

struct babblespace {
    pthread_mutex_t babble_mutex;  // mutex 
    pthread_cond_t babble_cond;    // condition variable
    int babble_first; // first message
    int babble_total; // total number of messages
    char babbles[BABBLE_LIMIT][BABBLE_LENGTH]; // messages
};

void display_messages(struct babblespace *shm) 
{
    pthread_mutex_lock(&shm->babble_mutex); // lock the shared memory
     
    printf("Messages:\n");
    int i = shm->babble_first;
    for (int k = 0; k < shm->babble_total; k++) 
    {
        printf("%s\n", shm->babbles[i]);
        i = (i + 1) % BABBLE_LIMIT;  // circular buffer
    }

    pthread_mutex_unlock(&shm->babble_mutex); // unlock the shared memory
}

void add_message(struct babblespace *shm) 
{
    char message[BABBLE_LENGTH];
    const char *initials = "alys";
    
    printf("Enter your message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';  //find newline and replace it with null terminator

    char message_with_initials[BABBLE_LENGTH];
    snprintf(message_with_initials, sizeof(message_with_initials), "%s: %s", initials, message); // format message with initials

    pthread_mutex_lock(&shm->babble_mutex); // lock the shared memory

    int i = (shm->babble_first + shm->babble_total) % BABBLE_LIMIT; // circular buffer
    strncpy(shm->babbles[i], message_with_initials, BABBLE_LENGTH - 1);  // copy the message to the shared memory and leave space for null terminator
    shm->babbles[i][BABBLE_LENGTH - 1] = '\0';  // add null terminator

    if (shm->babble_total < BABBLE_LIMIT) 
        shm->babble_total++;  // increment the total number of messages
    else 
        shm->babble_first = (shm->babble_first + 1) % BABBLE_LIMIT; // wrap around when the buffer is full

    pthread_cond_broadcast(&shm->babble_cond); // notify other processes
    pthread_mutex_unlock(&shm->babble_mutex); // unlock the shared memory
}

void child_process(struct babblespace *shm)
{
    while (1) 
    {
        pthread_mutex_lock(&shm->babble_mutex); // lock the shared memory
        pthread_cond_wait(&shm->babble_cond, &shm->babble_mutex); // wait for a signal

        
        printf("New messages detected:\n");
        display_messages(shm);

        pthread_mutex_unlock(&shm->babble_mutex); // unlock the shared memory
    }
}

int main() 
{
    struct babblespace *shm;

    int shm_fd = shm_open(BABBLE_NAME, O_RDWR | O_CREAT, BABBLE_MODE);
    
    ftruncate(shm_fd, sizeof(struct babblespace)); //resize shared memory
    
    shm = mmap(NULL, sizeof(struct babblespace), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0); 
   
    close(shm_fd); // close the file descriptor after mapping

    
    pthread_mutex_lock(&shm->babble_mutex);
    if (shm->babble_total == 0 && shm->babble_first == 0) 
    {
        pthread_mutex_init(&shm->babble_mutex, NULL);
        pthread_cond_init(&shm->babble_cond, NULL);
    }
    pthread_mutex_unlock(&shm->babble_mutex);

    pid_t pid = fork();
    

    if (pid == 0)
    {
       child_process(shm);
    } 
    else 
    {
        while (1) 
        {
            printf("Add a new message:\n");
            add_message(shm);
        }
    }

    munmap(shm, sizeof(struct babblespace)); // unmap the shared memory
    return 0;
}

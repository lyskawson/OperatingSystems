#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_THREADS 8

void *PrintHello(void *threadid)
{
    long taskid;
    sleep(1); 
    taskid = *(long *)threadid; 
    printf("Hello from thread %ld\n", taskid);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long taskids[NUM_THREADS]; // tablica na identyfikatory wątków

    for (long t = 0; t < NUM_THREADS; t++) {
        taskids[t] = t; // przypisanie ID
        printf("Creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)&taskids[t]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    pthread_exit(NULL); // Zakończenie procesu głównego
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//gcc -o myhello myhello.c -lpthread


void* thread_function(void* arg) 
{
   
    int thread_id = *(int*)arg;
    
    printf("Hello OpSys. Written by thread ID: %d\n", thread_id);
    pthread_exit(NULL);
}

int main() 
{
    const int NUM_THREADS = 10; 
    pthread_t threads[NUM_THREADS];
    int threads_ids[NUM_THREADS]; 

  
    for (int i = 0; i < NUM_THREADS; i++) 
    {
        threads_ids[i] = i + 1; 
        pthread_create(&threads[i], NULL, thread_function, &threads_ids[i]);        
    }


    for (int i = 0; i < NUM_THREADS; i++) 
    {
        pthread_join(threads[i], NULL);
    }

 
    return 0;
}


#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 600
#include <pthread.h>
#include <math.h>
#include <stdio.h> 

pthread_barrier_t barrier;

void* thread_function(void* arg) 
{
    int thread_id = *((int*)arg);

    pthread_barrier_wait(&barrier);
    
    double result = 0.0;
    for (int i = 1; i <= 1000; ++i) 
    {
        result += log(i);
    }

    printf("Hello OpSys. Written by thread ID: %d. Result: %f\n", thread_id, result);
    pthread_exit(NULL);
}

int main() 
{
    const int NUM_THREADS = 10; 
    pthread_t threads[NUM_THREADS];
    int threads_ids[NUM_THREADS]; 

    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) 
    {
        threads_ids[i] = i + 1; 
        pthread_create(&threads[i], NULL, thread_function, &threads_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) 
    {
        pthread_join(threads[i], NULL);
    }

  
    pthread_barrier_destroy(&barrier);
    return 0;
}
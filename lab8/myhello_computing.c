#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//gcc -o myhello myhello.c -lpthread -lm

void* thread_function(void* arg) 
{
   
    int thread_id = *(int*)arg;
    
    double result = 0.0;
    for (int i = 1; i < 1000; ++i) 
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

//a)
//Kolejność wykonywania wątków niekoniecznie będzie deterministyczna. 
//Wynika to z równoczesnego planowania wątków przez system operacyjny.
//Wynik potwierdza, że kolejność uruchamiania i wykonywania wątków 
//może być różna między kolejnymi uruchomieniami programu.

//c)
//Program bug5.c zawiera błąd związany z 
//przekazywaniem identyfikatora wątku (t) jako argumentu do funkcji PrintHello. 
//Błąd ten jest związany z tym, jak wątki i zmienne są współdzielone w pamięci.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>


//gcc -o prime_sum_mutex prime_sum_mutex.c -lpthread -lm

int N; /* liczba liczb pierwszych do znalezienia (arg.wywolania) */
int M = 2; /* poczatkowa liczba do sprawdzania (drugi arg.wywolania, domyslnie 2) */
int K = 1; /* liczba watkow obliczeniowych (trzeci arg.wywolania, domyslnie 1) */
int sprawdz; /* nastepna liczba do sprawdzania */
int znalezione = 0; /* liczba znalezionych i zsumowanych l.pierwszych */
long suma = 0; /* to bedzie obliczona suma liczb pierwszych */
pthread_mutex_t muteks_pobierania; 
pthread_mutex_t muteks_sumowania;  


int is_prime(int n) 
{
    if (n < 2) 
        return 0;
    for (int i = 2; i <= sqrt(n); i++) 
    {
        if (n % i == 0) 
            return 0;
    }
    return 1;
}


void *find_primes(void *arg) 
{
    while (1) 
    {
        int check;

        pthread_mutex_lock(&muteks_pobierania);
        if (znalezione >= N) 
        {
            pthread_mutex_unlock(&muteks_pobierania);
            break; //jeśli znaleziono wystarczajaca liczbe liczb pierwszych
        }
        check = sprawdz;
        sprawdz++;
        pthread_mutex_unlock(&muteks_pobierania);

        
        if (is_prime(check)) 
        {
            pthread_mutex_lock(&muteks_sumowania);
            if (znalezione < N) {
                suma = suma + check;
                znalezione++;
            }
            pthread_mutex_unlock(&muteks_sumowania);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) 
{
    sprawdz = M;
    N = atoi(argv[1]);
    if (argc >= 3) 
        M = atoi(argv[2]);
    if (argc == 4) 
        K = atoi(argv[3]);
 
    pthread_t threads[K];
    pthread_mutex_init(&muteks_pobierania, NULL);
    pthread_mutex_init(&muteks_sumowania, NULL);

    for (int i = 0; i < K; i++) 
    {
        pthread_create(&threads[i], NULL, find_primes, NULL);
            
    }

    for (int i = 0; i < K; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    printf("Sum: %ld\n",suma);

    pthread_mutex_destroy(&muteks_pobierania);
    pthread_mutex_destroy(&muteks_sumowania);

    return 0;
}

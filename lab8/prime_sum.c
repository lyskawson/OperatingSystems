#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//gcc -o prime_sum prime_sum.c -lm

int is_prime(long n) 
{
    if (n < 2) 
        return 0; 
    for (long i = 2; i <= sqrt(n); i++) 
    {
        if (n % i == 0) 
            return 0; 
    }
    return 1; 
}

int main(int argc, char *argv[]) 
{
    long N = atol(argv[1]);
    long M = atol(argv[2]);

    long sum = 0; 
    long count = 0;     
    long curr = M;   

    while (count < N) 
    {
        if (is_prime(curr)) 
        {
            sum = sum + curr;
            count++;        
        }
        curr++;          
    }

    printf("Sum: %ld\n",sum);
    return 0;
}

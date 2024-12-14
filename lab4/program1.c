

#include <stdio.h>
#include <time.h>    

int main() 
{
   
    int i = 0;
    while (1) 
    {
        i++;
        printf("%d\n", i);
        struct timespec remaining, request = {0, 10000000}; // 10 ms
        nanosleep(&request, &remaining);
    }
    return 0;
}

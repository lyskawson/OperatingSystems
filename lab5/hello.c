#include <stdio.h>
#include <stdlib.h>

int main() 
{
    
    char *username = getenv("LOGNAME");

    if (username == NULL) 
    {
        username = "user";
    }

    printf("Hello, %s!\n", username);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define MAX_SONS 100 
#define MAX_ROWS 100
#define MAX_COLS 100

int **territories;
int rows, cols, sons, chances; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int finished_sons = 0; 

typedef struct
{
    int id;          
    int captured;    
} Son;

int generate_number(int min, int max)
{
    int random_num = min + (int)((max - min) * drand48());
    return random_num;
}

void initialize_territory()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            territories[i][j] = -1;
        }
    }
}

void draw_territory()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if(territories[i][j] == -1)
                printf("-");
            else 
                printf("%d ", territories[i][j]);

        }
        printf("\n");
    }
    printf("\n");
}

void *sons_funkcja(void *arg)
{
    Son *data = (Son *)arg;
    for (int i = 0; i < chances; i++)
    {
        int x = generate_number(0, rows);
        int y = generate_number(0, cols);

        pthread_mutex_lock(&mutex);
        if (territories[x][y] == -1) 
        {
            territories[x][y] = data->id;
            data->captured++;
        }
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    
    finished_sons++;
    pthread_cond_signal(&cond); // notify rejent
    
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *rejent_funkcja(void *arg)
{
    Son *son_data = (Son *)arg;
    int occupied_territories = 0;

    for (int i = 0; i < sons; i++)
    {
        pthread_mutex_lock(&mutex);
        while (finished_sons <= i)
        {
            pthread_cond_wait(&cond, &mutex);
        }

        printf("Son [%d] captured %d territories.\n", son_data[i].id, son_data[i].captured);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (territories[i][j] != -1)
            {
                occupied_territories++;
            }
        }
    }
    int unoccupied = rows * cols - occupied_territories;
    printf("Unoccupied territories: %d.\n", unoccupied);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    srand48(time(NULL)); 

    pthread_t rejent;
    pthread_t son_threads[MAX_SONS];
    Son son[MAX_SONS];

    sons = 4; 
    chances = 6;
    rows = 5; 
    cols = 5; 

    // Inicjalizacja tablicy territories
    territories = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        territories[i] = (int *)malloc(cols * sizeof(int));
    }

    initialize_territory();

    // Tworzenie watkow sons
    for (int i = 0; i < sons; i++)
    {
        son[i].id = i;
        son[i].captured = 0;
        pthread_create(&son_threads[i], NULL, sons_funkcja, &son[i]);
    }

    // Tworzenie watku rejent
    pthread_create(&rejent, NULL, rejent_funkcja, son);

    // Oczekiwanie na zakonczenie watkow sons
    for (int i = 0; i < sons; i++)
    {
        pthread_join(son_threads[i], NULL);
    }

    // Oczekiwanie na zakonczenie watku rejent
    pthread_join(rejent, NULL);

    draw_territory();

    // Zwolnienie pamieci dla tablicy territories
    for (int i = 0; i < rows; i++)
    {
        free(territories[i]);
    }
    free(territories);

    return 0;
}

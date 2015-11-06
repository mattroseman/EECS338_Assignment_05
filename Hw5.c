#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define KEY 64043

#define NUM_SEM 4

#define TRUE 1
#define FALSE 0

void Writer();
void Reader();

/* Semaphores */
sem_t mutex, rmutex, rdr, wtr;

int semid;

/* Integers */
unsigned int nwriters;
unsigned int nreaders;

/* Booleans */
unsigned int Busy;
unsigned int RBlocked;

unsigned int numThreads;

unsigned int sleepScale;

int main(int argc, char *argv[])
{
    sleepScale = 1;

    if (argc > 1)
    {
        if (sscanf(argv[1], "%d", &numThreads) < 0)
        {
            perror("sscanf failed\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        numThreads = 10;
    }

/* Initialize Semaphores */
    if (sem_init(&mutex, 0, (unsigned int)1) < 0)
    {
        perror("sem_init for mutex failed\n");
        exit(EXIT_FAILURE);
    }

    printf("The semaphore mutex has been initialized with value 1\n");

    if (sem_init(&rmutex, 0, (unsigned int)0) < 0)
    {
        perror("sem_init for rmutex failed\n");
        exit(EXIT_FAILURE);
    }

    printf("The semaphore rmutex has been initialized with value 0\n");

    if (sem_init(&wtr, 0, (unsigned int)0) < 0)
    {
        perror("sem_init for wtr failed\n");
        exit(EXIT_FAILURE);
    }

    printf("The semaphore wtr has been initialized with value 0\n");

    if (sem_init(&rdr, 0, (unsigned int)0) < 0)
    {
        perror("sem_init for rdr failed\n");
        exit(EXIT_FAILURE);
    }

    printf("The semaphore rdr has been initialized with value 0\n");

/* Initialize Variables */
    nwriters = 0;
    nreaders = 0;
    Busy = FALSE;
    RBlocked = FALSE;

    int i;
    unsigned int seed;
    if ((seed = time(NULL)) < 0)
    {
        perror("time failed\n");
        exit(EXIT_FAILURE);
    }
    srand(seed);

    for (i = 0; i < numThreads; i++)
    {
        unsigned int process = rand()%2;
        sleep(2 * sleepScale);
        if (process == 0)
        {
            // start read
        }
        if (process == 1)
        {
            // start write
        }
    }

    exit(EXIT_SUCCESS);
}

void Reader()
{
    sem_wait(&rmutex);
    sem_wait(&mutex);

    if (nwriters > 0)
    {
        RBlocked = TRUE;
        sem_post(&mutex);
        sem_wait(&rdr);
    }
    else
    {
        nreaders++;
        sem_post(&mutex);
    }
    sem_post(&rmutex);
    /* READ */
    sem_wait(&mutex);
    nreaders--;
    if (nreaders == 0 && nwriters > 0)
    {
        Busy = TRUE;
        sem_post(&wtr);
    }
    sem_post(&mutex);
    /* DO-SOMETHING */
}

void Writer()
{
    sem_wait(&mutex);
    nwriters++;
    if (Busy || nreaders > 0)
    {
        sem_post(&mutex);
        sem_wait(&wtr);
    }
    else
    {
        Busy = TRUE;
        sem_post(&mutex);
    }
    /* WRITE */
    sem_wait(&mutex);
    nwriters--;
    Busy = FALSE;
    if (nwriters > 0)
    {
        Busy = TRUE;
        sem_post(&wtr);
    }
    else if (RBlocked)
    {
        RBlocked = FALSE;
        nreaders++;
        sem_post(&rdr);
    }
    sem_post(&mutex);
    /* DO-SOMETHING */
}

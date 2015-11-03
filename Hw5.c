#include "Semaphore.h"

#define NUM_SEM 4

#define MUTEX 0
#define RMUTEX 1
#define WTR 2
#define RDR 3

void Writer();
void Reader();

/* Semaphores Initial Values */
unsigned int InitVal[] = {1,0,0,0};

/* Integers */
unsigned int nwriters;
unsigned int nreaders;

/* Booleans */
unsigned int Busy;
unsigned int RBlocked;

int semid;

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
    semid = CreateGroup(KEY, NUM_SEM, InitVal);

/* Initialize Variables */
    nwriters = 0;
    nreaders = 0;
    Busy = 0;
    RBlocked = 0;

    int i;
    unsigned int seed;
    if ((seed = time(NULL)) < 0)
    {
        perror("time failed\n");
        exit(EXIT_FAILURE):
    }
    if (srand(seed) < 0)
    {
        perror("srand failed\n");
        exit(EXIT_FAILURE):
    }
    for (i = 0; i < numThreads, i++)
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

}

void Writer()
{

}

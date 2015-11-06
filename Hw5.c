#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define TRUE 1
#define FALSE 0

void *Writer(void *);
void *Reader(void *);

/* Semaphores */
sem_t mutex, wtr;

int semid;

/* Threads */
int rc;

pthread_attr_t attr;

/* Integers */
unsigned int readcount;

/* Booleans */
unsigned int numThreads;

/* Misc. */
unsigned int sleepScale;

char * signature;

int main(int argc, char *argv[])
{
    sleepScale = 1;

    signature = malloc(50 * sizeof(char));
    if (sprintf(signature, "--- (main) TID %ld: ", syscall(SYS_gettid)) < 0)
    {
        perror ("sprintf failed\n");
        exit(EXIT_FAILURE);
    }

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
        numThreads = 5;
    }

/* Initialize Semaphores */
    if (sem_init(&mutex, 0, (unsigned int)1) < 0)
    {
        perror("sem_init for mutex failed\n");
        exit(EXIT_FAILURE);
    }

    printf("%sThe semaphore mutex has been initialized with value 1\n", signature);

    if (sem_init(&wtr, 0, (unsigned int)1) < 0)
    {
        perror("sem_init for wtr failed\n");
        exit(EXIT_FAILURE);
    }

    printf("%sThe semaphore wtr has been initialized with value 1\n", signature);

/* Initialize Variables */
    readcount = 0;

    int i;
    unsigned int seed;
    if ((seed = time(NULL)) < 0)
    {
        perror("time failed\n");
        exit(EXIT_FAILURE);
    }
    srand(seed);

    printf("%sThe random number generator has been seeded\n", signature);

/* Initialize Threads */
    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));

    if ((rc = pthread_attr_init(&attr)) != 0)
    {
        perror("pthread_attr_init() failed\n");
        printf("return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    if ((rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE)) != 0)
    {
        perror("pthread_attr_setdetachstate() failed\n");
        printf("return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    printf("%sThe pthread attributes have been set\n", signature);

/* Start Threads */
    for (i = 0; i < numThreads; i++)
    {
        unsigned int process = rand()%2;
        sleep(2 * sleepScale);
        if (process == 0)
        {
            // Start Reader
            if ((rc = pthread_create(&threads[i], NULL, &Reader, &i)) != 0)
            {
                perror("pthread_create() for Reader failed\n");
                printf("return code %d\n", rc);
                exit(EXIT_FAILURE);
            }
        }
        if (process == 1)
        {
            // Start Writer
            if ((rc = pthread_create(&threads[i], NULL, &Writer, &i)) != 0)
            {
                perror("pthread_create() for Writer failed\n");
                printf("return code %d\n", rc);
                exit(EXIT_FAILURE);
            }
        }
    }

/* Cleanup */
    // Wait for all the threads to finish and rejoin the main thread
    for (i = 0; i < numThreads; i++)
    {
        if((rc = pthread_join(threads[i], NULL)) != 0)
        {
            perror("pthread_join() failed\n");
            printf("return code %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }

    free(threads);

    if ((rc = pthread_attr_destroy(&attr)) != 0)
    {
        perror("pthread_attr_destroy() failed\n");
        printf("return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    if (sem_destroy(&mutex) < 0)
    {
        perror("sem_destroy on mutex failed\n");
        exit(EXIT_SUCCESS);
    }
    if (sem_destroy(&wtr) < 0)
    {
        perror("sem_destroy on wtr failed\n");
        exit(EXIT_SUCCESS);
    }

    free(signature);

    exit(EXIT_SUCCESS);
}

void *Reader(void *arg)
{
    int threadNum = *(int *)arg;

    char * rsignature = malloc(50 * sizeof(char));
    if (sprintf(rsignature, "--- [%d] (reader) TID %ld: ", threadNum, syscall(SYS_gettid)) < 0)
    {
        perror ("sprintf failed for rsignature\n");
        exit(EXIT_FAILURE);
    }
    char * csrsignature = malloc(50 * sizeof(char));
    if (sprintf(csrsignature, "*** [%d] (reader) TID %ld: ", threadNum, syscall(SYS_gettid)) < 0)
    {
        perror ("sprintf failed for csrsignature\n");
        exit(EXIT_FAILURE);
    }

    printf("%sReader has started\n", rsignature);
    sleep(2 * sleepScale);

    sem_wait(&mutex);
    printf("%sEntering Critical Section\n", csrsignature);
    sleep(2 * sleepScale);
    readcount++;
    printf("%sthere are now %d readers reading\n", csrsignature, readcount);
    sleep(2 * sleepScale);
    if (readcount == 1)
    {
        sem_wait(&wtr);
    }
    printf("%sExiting Critical Section\n", csrsignature);
    sleep(2 * sleepScale);
    sem_post(&mutex);

    /* Read */
    printf("%snow reading\n", rsignature);
    sleep(4 * sleepScale);

    sem_wait(&mutex);
    printf("%sEntering Critical Section\n", csrsignature);
    sleep(2 * sleepScale);
    readcount--;
    printf("%sthere are now %d readers reading\n", csrsignature, readcount);
    sleep(2 * sleepScale);
    if (readcount == 0)
    {
        sem_post(&wtr);
    }
    printf("%sExiting Critical Section\n", csrsignature);
    sleep(2 * sleepScale);
    sem_post(&mutex);

    printf("%sReader done\n", rsignature);

    free(rsignature);
    free(csrsignature);
    pthread_exit(NULL);
}

void *Writer(void *arg)
{
    int threadNum = *(int *)arg;

    char * wsignature = malloc(50 * sizeof(char));
    if (sprintf(wsignature, "--- [%d] (writer) TID %ld: ", threadNum, syscall(SYS_gettid)) < 0)
    {
        perror ("sprintf failed for wsignature\n");
        exit(EXIT_FAILURE);
    }
    char * cswsignature = malloc(50 * sizeof(char));
    if (sprintf(cswsignature, "*** [%d] (writer) TID %ld: ", threadNum, syscall(SYS_gettid)) < 0)
    {
        perror("sprintf failed for cswsignature\n");
        exit(EXIT_FAILURE);
    }

    sem_wait(&wtr);
    printf("%sEntering Critical Section\n", cswsignature);
    sleep(2 * sleepScale);

    /* Write */
    printf("%snow writing\n", cswsignature);
    sleep(4 * sleepScale);

    printf("%sExiting Critical Section\n", cswsignature);
    sleep(2 * sleepScale);
    sem_post(&wtr);

    printf("%sWriter done\n", wsignature);

    free(wsignature);
    free(cswsignature);
    pthread_exit(NULL);
}

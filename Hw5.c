#include "Semaphore.h"

#define NUM_SEM 4

#define MUTEX 0
#define RMUTEX 1
#define WTR 2
#define RDR 3

/* Semaphores Initial Values */
unsigned int InitVal[] = {1,0,0,0};

/* Integers */
unsigned int nwriters;
unsigned int nreaders;

/* Booleans */
unsigned int Busy;
unsigned int RBlocked;

int main()
{

}

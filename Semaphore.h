#ifndef Semaphore_h
#define Semaphore_h

int CreateGroup(key_t, int, unsigned short *);

int GetGroup(key_t);

void DestroyGroup(int);

// Signal takes in the semaphore group id and the semaphore number in that group
void Signal(int, int);

// Wait takes same arguments as Signal
void Wait(int, int);

// Get Val takes in semaphore group id and semaphore number and returns that semaphore number
int GetVal(int, int);

#endif

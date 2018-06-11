// sema1_mod1.cpp
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <iostream>
#include <stdio.h>
using namespace std;
// Semaphore id.
static int sem_id;

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
// Union semun is defined by including <sys/semh.h>
 #else // Accroding to X/OPEN we have to define it ourselves.
union semun { int val;// Value for SETVAL.
        struct semid_ds *buf; // Buffer for IPC_STAT, IPC_SET
unsigned short *array;
// Array for GETALL, SETALL.
 struct seminfo *__buf; // Buffer forIPC_INFO (Linux).
};
#endif


// Initializes semaphore using SETVAL.
static int set_semvalue(int val)
{
union semun sem_union; sem_union.val = val; if (semctl(sem_id, 0, SETVAL,
sem_union) == -1)
{
return 0;
} return 1;
}
// Delete semaphore.
static int del_semvalue()
{
union semun sem_union; sem_union.val = 1;
if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
{ return 0; } return 1;
}
// Decrements semaphore.
 static int SEM_DOWN()
{
        struct sembuf b;
b.sem_num = 0;
b.sem_op = -1;
b.sem_flg = SEM_UNDO; if (semop(sem_id, &b, 1) == -1)
{
cout << "Semaphore DOWN() failed!" << endl; return 0; } return 1;
}
// Increments sempahore.
 static int SEM_UP()
{ struct sembuf b;
b.sem_num = 0;
b.sem_op = 1;
b.sem_flg = SEM_UNDO; if (semop(sem_id, &b, 1) == -1)
{
cout << "Sempahore UP() failed!" << endl; return 0; } return 1;
}
// Program entry point.
int main(int argc, char *argv[])
{
int i, pause_time, arg; char ce = 'e', cl = 'l';
// Seed RNG with process ID.
 srand((unsigned int)getpid());
// Create a semaphore.

sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
// Check if any argument was passed.
if (argc > 1)
{
// Get the argument and convert to an integer.
 arg = atoi(argv[1]);
// If 1, proceed normally.
if (arg == 1) {
if (!set_semvalue(1))
{
cout << "Semaphore initialzation failed!" << endl;
exit(EXIT_FAILURE);


ce = 'E';
cl = 'L';
sleep(1);
} } else {
if (!set_semvalue(0))
{
cout << "Semaphore initialization failed!" << endl;
exit(EXIT_FAILURE);
} sleep(1);
}
// Enter and leave critical section 10 times. for (i = 0; i < 10; i++)
{
if (!SEM_DOWN())
{
exit(EXIT_FAILURE);
}
// Entering critical section.
cout << ce; fflush(stdout);
// Simulating critical section.
pause_time = rand() % 3; sleep(pause_time);
// Leaving critical section.
cout << cl; fflush(stdout);
if (!SEM_UP())
{
exit(EXIT_FAILURE);
}
pause_time = rand() % 2; sleep(pause_time);
}
cout << endl << getpid() << " finished!" << endl; if (argc > 0)
{ sleep(2); del_semvalue();
}
exit(EXIT_SUCCESS);
}
}

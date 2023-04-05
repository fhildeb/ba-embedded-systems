#include "advanced-lock.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* union semun is defined by including <sys/sem.h> */
#else
/* according to X/OPEN we have to define it ourselves */
union semun
{
  int val;                   /* value for SETVAL */
  struct semid_ds *buf;      /* buffer for IPC_STAT, IPC_SET */
  unsigned short int *array; /* array for GETALL, SETALL */
  struct seminfo *__buf;     /* buffer for IPC_INFO */
};
#endif

int setReadLock(int semid)
{
  struct sembuf sops[] = {{0, 1, 0}, {1, 0, 0}};
  // Lesesemaphor erhöhen und warten, bis Schreibsemaphor 0 ist

  return semop(semid, sops, 2);
}

int releaseReadLock(int semid)
{
  struct sembuf sops[] = {{0, -1, 0}};
  // Lesesemaphor verringern

  return semop(semid, sops, 1);
}

int setWriteLock(int semid)
{
  struct sembuf sops1[] = {{1, 0, 0}, {1, 1, 0}};
  // warten bis Schreibsemaphor 0 ist, Schreibsemaphor erhöhen

  struct sembuf sops2[] = {{0, 0, 0}};
  // Warten, bis Lesesemaphor 0 ist

  int res = semop(semid, sops1, 2);
  if (res < 0)
    return res;
  return semop(semid, sops2, 1);
}

int releaseWriteLock(int semid)
{
  struct sembuf sops[] = {{1, -1, 0}};
  // Schreibsemaphor verringern

  return semop(semid, sops, 1);
}

int initReadWriteLock(key_t key) // liefert semid
{
  union semun seminit;
  unsigned short seminitval[] = {0, 0};
  int semid, erg;
  /* Semaphorgruppe anfordern: 2 Semaphore SHM Lesen/Schreiben */
  if ((semid = semget(key, 2, IPC_CREAT | IPC_EXCL | 0600)) < 0)
  {
    perror("semget");
    exit(EXIT_FAILURE);
  }

  seminit.array = seminitval;

  if ((erg = semctl(semid, 0, SETALL, seminit)) < 0)
  {
    perror("semctl");
    exit(EXIT_FAILURE);
  }
  return semid;
}

int getIdReadWriteLock(key_t key) // liefert semid
{
  int semid;
  if ((semid = semget(key, 2, 0)) < 0)
  {
    perror("semget");
    exit(EXIT_FAILURE);
  }
  return semid;
}

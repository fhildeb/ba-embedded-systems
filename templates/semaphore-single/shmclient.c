#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#define SHMSZ 256
#define NSEMS 1

int main(int argc, char **argv)
/* Demo für Nutzung des Shared Memory */
{
  int shmid, semid;
  key_t key, semkey;
  char *shm;

  struct sembuf ops;

  if (argc != 3)
  {
    fprintf(stderr, "Aufruf: shmclient <servername> <mesg>\n Server wird "
                    "gestoppt bei mesg 0\n");
    return EXIT_FAILURE;
  }

  if ((key = ftok(argv[1], '1')) < 0)
  {
    perror("ftok shm");
    return EXIT_FAILURE;
  }

  if ((semkey = ftok(argv[1], '2')) < 0)
  {
    perror("ftok sem");
    return EXIT_FAILURE;
  }

  /* Shared memory anfordern, Besitzer und Gruppe dürfen lesen und
     Schreiben, muss existieren (Server up) */
  if ((shmid = shmget(key, SHMSZ, 0660)) < 0)
  {
    perror("shmget");
    return EXIT_FAILURE;
  }

  /* dito für Semaphorgruppe mit einem Semaphor */

  if ((semid = semget(semkey, NSEMS, 0660)) < 0)
  {
    perror("semget");
    return EXIT_FAILURE;
  }

  /* Shared Memory in Adressraum einblenden */
  if ((shm = shmat(shmid, NULL, 0)) == (char *)(-1))
  {
    perror("shmat");
    return EXIT_FAILURE;
  }

  /* exklusiven Zugriff auf Shared Memory sichern */
  ops.sem_num = 0;
  ops.sem_op = -1;
  ops.sem_flg = 0;
  if (semop(semid, &ops, 1) == -1)
  {
    perror("semop P");
    exit(EXIT_FAILURE);
  }

  strncpy(shm, argv[2], SHMSZ);

  /* exklusiven Zugriff wieder freigeben */
  ops.sem_num = 0;
  ops.sem_op = +1;
  ops.sem_flg = 0;
  if (semop(semid, &ops, 1) == -1)
  {
    perror("semop P");
    exit(EXIT_FAILURE);
  }

  /* Shared Memory abkoppeln */
  if (shmdt(shm) < 0)
  {
    perror("shmdt");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

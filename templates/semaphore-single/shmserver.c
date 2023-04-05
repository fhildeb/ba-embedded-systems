#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#define SHMSZ 256
#define NSEMS 1

int shmid;
int semid;

/* bei Programmende werden SHM-Segment und Sema gelöscht */
void exithandler()
{

  if (shmctl(shmid, IPC_RMID, NULL) < 0)
  {
    perror("msgctl");
    _exit(EXIT_FAILURE);
  }

  if (semctl(semid, 0, IPC_RMID, NULL) < 0)
  {
    perror("semctl");
    _exit(EXIT_FAILURE);
  }

  return;
}

/* bei SIGINT werden SHM-Segment und Sema gelöscht */
void sighandler(int signo)
{
  exithandler();
  _exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
/* Demo für Nutzung des Shared Memory,
   Zugriffssteuerung mit Semaphoren */
{
  key_t key, semkey;
  char *shm;
  struct sigaction old, new;
  struct sembuf sops;

  if (argc != 1)
  {
    fprintf(stderr, "Aufruf: shmserver \n");
    return EXIT_FAILURE;
  }

  /* Signalbehandlung vorbereiten */
  sigemptyset(&(new.sa_mask));
  new.sa_handler = sighandler;
  new.sa_flags = 0;

  if (sigaction(SIGINT, &new, &old) < 0)
  {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  key = ftok(argv[0], '1');
  assert(key != -1);
  semkey = ftok(argv[0], '2');
  assert(semkey != -1);
  /* Shared memory anfordern, Besitzer und Gruppe dürfen lesen und
     Schreiben */
  if ((shmid = shmget(key, SHMSZ, 0660 | IPC_CREAT)) < 0)
  {
    perror("shmget");
    return EXIT_FAILURE;
  }

  /* Semaphor anfordern */
  if ((semid = semget(semkey, NSEMS, 0660 | IPC_CREAT)) < 0)
  {
    perror("semget");
    return EXIT_FAILURE;
  }

  /* Exit-Handler installieren, damit SHM in jedem Falle freigegeben wird */
  atexit(exithandler);

  /* Semaphor initialisieren */
  if (semctl(semid, 0, SETVAL, 1) < 0)
  {
    perror("semctl/seminit");
    return EXIT_FAILURE;
  }

  /* Shared Memory in Adressraum einblenden */
  if ((shm = shmat(shmid, NULL, 0)) == (char *)(-1))
  {
    perror("shmat");
    return EXIT_FAILURE;
  }

  /* exklusiven Zugriff auf Shared Memory sichern */
  sops.sem_num = 0;
  sops.sem_op = -1;
  sops.sem_flg = 0;
  if (semop(semid, &sops, 1) == -1)
  {
    perror("semop P");
    exit(EXIT_FAILURE);
  }

  /* maximal SHMSZ Bytes in SHM kopieren */
  strncpy(shm, "Hello World!", SHMSZ);

  /* Zeichenkette in SHM ausgeben */
  while (shm[0] != '0')
  {
    puts(shm);
    /* exklusiven Zugriff wieder freigeben */
    sops.sem_num = 0;
    sops.sem_op = +1;
    sops.sem_flg = 0;
    if (semop(semid, &sops, 1) == -1)
    {
      perror("semop P");
      exit(EXIT_FAILURE);
    }

    sleep(1);

    /* exklusiven Zugriff auf Shared Memory sichern */
    sops.sem_num = 0;
    sops.sem_op = -1;
    sops.sem_flg = 0;
    if (semop(semid, &sops, 1) == -1)
    {
      perror("semop P");
      exit(EXIT_FAILURE);
    }
  }

  /* exklusiven Zugriff wieder freigeben */
  sops.sem_num = 0;
  sops.sem_op = +1;
  sops.sem_flg = 0;
  if (semop(semid, &sops, 1) == -1)
  {
    perror("semop P");
    exit(EXIT_FAILURE);
  }

  if (shmdt(shm) < 0)
  {
    perror("shmdt");
    return EXIT_FAILURE;
  }

  /* SHM Löschen in Exit-Funktion! */
  return EXIT_SUCCESS;
}

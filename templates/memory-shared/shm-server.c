#include "shm.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

int shmid;

/* bei SIGINT wird SHM-Segment gelöscht */
void sighandler(int signo) { exit(EXIT_FAILURE); }

void exithandler()
{

  if (shm_unlink(SEGNAME) < 0)
  {
    perror("shm_unlink");
    _exit(EXIT_FAILURE);
  }

  return;
}

int main(int argc, char **argv)
/* Demo für Nutzung des Shared Memory */
/* Achtung, keine Synchronisation des */
/* Zugriffs, das ist ein Fehler !!!   */
{
  char *shm;
  struct sigaction old, new;

  if (argc != 1)
  {
    fprintf(stderr, "Aufruf: shmserver \n");
    return EXIT_FAILURE;
  }

  /* Shared memory anfordern, Besitzer und Gruppe dürfen lesen und
     Schreiben */
  if ((shmid = shm_open(SEGNAME, O_CREAT | O_RDWR, 0660)) < 0)
  {
    perror("shm_open");
    return EXIT_FAILURE;
  }

  /* Exit-Handler installieren, damit SHM in jedem Falle freigegeben wird */
  atexit(exithandler);

  /* Signalbehandlung vorbereiten und installieren */
  sigemptyset(&(new.sa_mask));
  new.sa_handler = sighandler;
  new.sa_flags = 0;

  if (sigaction(SIGINT, &new, &old) < 0)
  {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  /* Größe festlegen */
  if (ftruncate(shmid, SHMSZ) < 0)
  {
    perror("ftruncate");
    return EXIT_FAILURE;
  }

  /* Shared Memory in Adressraum einblenden */
  if ((shm = mmap(NULL, SHMSZ, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0)) ==
      MAP_FAILED)
  {
    perror("mmap");
    return EXIT_FAILURE;
  }

  /* maximal SHMSZ Bytes in SHM kopieren */
  strncpy(shm, "Hello World!", SHMSZ);

  /* Zeichenkette in SHM ausgeben */
  while (shm[0] != '0')
  {
    puts(shm);
    sleep(1);
  }

  /* SHM Löschen in Exit-Funktion! */
  return EXIT_SUCCESS;
}

#include "shm.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
/* Demo für Nutzung des Shared Memory */
/* Achtung, keine Synchronisation !!  */
/* Das ist ein Fehler !!!             */
{
  int shmid;

  char *shm;

  if (argc != 2)
  {
    fprintf(stderr, "Aufruf: shmclient mesg\n Server wird gestoppt bei mesg 0");
    return EXIT_FAILURE;
  }

  /* Shared memory anfordern, Besitzer und Gruppe d�ürfen lesen und
     Schreiben, muss existieren (Server up) */
  if ((shmid = shm_open(SEGNAME, O_RDWR, 0660)) < 0)
  {
    perror("shm_open");
    return EXIT_FAILURE;
  }

  /* Shared Memory in Adressraum einblenden */
  if ((shm = mmap(NULL, SHMSZ, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0)) ==
      MAP_FAILED)
  {
    perror("mmap");
    return EXIT_FAILURE;
  }

  strncpy(shm, argv[1], SHMSZ);

  close(shmid);

  return EXIT_SUCCESS;
}

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "readerwriter.h"
#define SHMSZ 256

int main (int argc, char **argv)
/* Demo für Leser-Schreiber-Problem, Schreiber */

{
    int shmid, semid;
    key_t key;
    char *shm;

    if(argc != 3) {
        fprintf(stderr, "Aufruf: %s <readername> <mesg>\n Server wird gestoppt bei mesg 0\n", argv[0]);
	return EXIT_FAILURE;
    }

    if((key = ftok(argv[1], 1)) < 0) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    /* Shared memory anfordern, Besitzer und Gruppe dürfen lesen und
       Schreiben, muss existieren (Server up) */
    if ((shmid = shmget(key, SHMSZ, 0660)) < 0) {
	perror("shmget");
	return EXIT_FAILURE;
    }

    /* Shared Memory in Adressraum einblenden */
    if ((shm = shmat(shmid, NULL, 0)) == (char *)(-1)) {
	perror("shmat");
	return EXIT_FAILURE;
    }

    /* Semaphoren initialisieren */
    semid = getIdReadWriteLock(key);

/*----- WriteLock setzen -----------------------------*/
    if(setWriteLock(semid)<0) {
         perror("semaphor p()");
         exit(EXIT_FAILURE);
    }

    puts("WriteLock gesetzt!");
    strncpy(shm, argv[2], SHMSZ);
    sleep(5);

/*----- WriteLock freigeben -------------------------*/
    if(releaseWriteLock(semid)<0) {
         perror("semaphor p()");
         exit(EXIT_FAILURE);
    }
    puts("WriteLock freigegeben!");
    
    if(shmdt(shm)<0) {
    	perror("shmdt");
	return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}



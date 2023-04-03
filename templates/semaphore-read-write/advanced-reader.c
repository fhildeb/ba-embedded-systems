#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include "advanced-lock.h"
#define SHMSZ 256

int shmid, semid;

/* Bei Programmende wird SHM-Segment gelöscht */
void exithandler()
{

   if(shmctl(shmid, IPC_RMID, NULL) < 0) {
     perror("shmctl");
     _exit(EXIT_FAILURE);
   }

   if(semctl(semid, 2, IPC_RMID, NULL) < 0) {
     perror("semctl");
     _exit(EXIT_FAILURE);
   }
   return;
}


/* bei SIGINT wird SHM-Segment gelöscht */
void sighandler(int signo)
{
   exithandler();
   _exit(EXIT_FAILURE);
}

int main (int argc, char **argv)
/* Demo für Nutzung des Shared Memory */
/* Achtung, keine Synchronisation !!  */
/* Das ist ein Fehler !!!             */
{
    key_t key;
    char *shm;
    struct sigaction old, new;
    int count=0;

    if(argc > 2) {
        fprintf(stderr, "Aufruf: %s  für ersten Leser, %s 1 für jeden weiteren!\n", argv[0], argv[0]);
	return EXIT_FAILURE;
    }

    /* Signalbehandlung vorbereiten */
    sigemptyset(&(new.sa_mask));
    new.sa_handler = sighandler;
    new.sa_flags = 0;


    if((key = ftok(argv[0], 1)) < 0) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    /* Shared memory anfordern, Besitzer und Gruppe dürfen lesen und
       Schreiben */
    if ((shmid = shmget(key, SHMSZ, 0660|IPC_CREAT)) < 0) {
	perror("shmget");
	return EXIT_FAILURE;
    }

    if(argc == 1) {
        semid = initReadWriteLock(key);
        /* Signalhandler scharf schalten */
        if(sigaction(SIGINT, &new, &old) < 0) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }
        /* Exit-Handler installieren, damit SHM und Semaphoren in jedem Falle freigegeben werden */
        atexit(exithandler);

    } else {
        /* keine Signalbehandlung */
        semid = getIdReadWriteLock(key);
    }
    

    /* Shared Memory in Adressraum einblenden */
    if ((shm = shmat(shmid, NULL, 0)) == (char *)(-1)) {
	perror("shmat");
	return EXIT_FAILURE;
    }

    /* maximal SHMSZ Bytes in SHM kopieren */
    strncpy(shm, "Hello World!", SHMSZ);

    /* Zeichenkette in SHM ausgeben */
    while(shm[0] != '0') {
        if(setReadLock(semid)<0) {
            perror("semaphor p()");
            exit(EXIT_FAILURE);
        }
    	printf("%04d: %s\n", count++, shm);
	sleep(1);
        if(releaseReadLock(semid)<0) {
            perror("semaphor p()");
            exit(EXIT_FAILURE);
        }

    }

    if(shmdt(shm)<0) {
    	perror("shmdt");
	return EXIT_FAILURE;
    }

    /* SHM und SEM Löschen in Exit-Funktion! */
    return EXIT_SUCCESS;
}


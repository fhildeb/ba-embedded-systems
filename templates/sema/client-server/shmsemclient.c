#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#define SHMSZ 256
#define NSEM 3

int main (int argc, char **argv)
/* Client auf Basis Shared Memory / Semaphoren. 
   Arbeitsweise:
   -  Client sichert über Semaphor 0 exklusiven Zugriff auf Server
   -  Client schreibt Auftrag (argv[1]) in Shared Memory. 
   -  Client signalisiert an Semaphor 1, dass er
      im SHM-Bereich Nachricht hinterlegt hat. 
   -  Client wartet am Semaphor 2 auf Ergebnis vom Server
   -  Client gibt exklusiven Serverzugriff frei 
*/

{
    int semid, shmid, zaehler=0, done;
    key_t key;
    char *shm;
    unsigned short semvals[5];
    struct sembuf semops;

    if(argc != 3) {
        fprintf(stderr, "usage: shmsemclient <shmsemserv> <mesg>\n use mesg 0 to stop server\0");
	return 1;
    }
    
    
    /* Key für shm und sem aus Servernamen ableiten */
    if((key = ftok(argv[1], '1')) < 0) {
    	perror("ftok");
    	return EXIT_FAILURE;
    }

    /* Shared memory anfordern, Besitzer und Gruppe dürfen lesen und 
       Schreiben, muss existieren (Server up) */  
    if ((shmid = shmget(key, SHMSZ, 0660)) < 0) {
	perror("shmget");
	return EXIT_FAILURE;
    }	
    
    /* Semaphorgruppe aus NSEM (3) Semaphoren anfordern */
    if ((semid = semget(key, NSEM, 0)) < 0) {
	perror("semget");
	return EXIT_FAILURE;
    }
       
    /* Shared Memory in Adressraum einblenden */
    if ((shm = shmat(shmid, NULL, 0)) == (char *)(-1)) {
	perror("shmat");
	return EXIT_FAILURE;
    }
   
    /* exclusiven Zugriff auf Server sichern P(0)*/
    semops.sem_num=0;
    semops.sem_op=-1;
    semops.sem_flg=0;
    if (semop(semid, &semops, 1)==-1) {
        perror("semop1");
        return EXIT_FAILURE;
    }
	
    strncpy(shm, argv[2],SHMSZ);
	
     /* Server informieren  V(1)*/
     semops.sem_num=1;
     semops.sem_op=1;
     semops.sem_flg=0;

     if (semop(semid, &semops, 1)==-1) {
           perror("semop2");
           return EXIT_FAILURE;
     }
   
     
     /* nur wenn kein Serverende */
     
     if (argv[2][0]!= '0' ) {
       /* auf Antwort warten P(2) */
       semops.sem_num=2;
       semops.sem_op=-1;
       semops.sem_flg=0;		
 
       if (semop(semid, &semops, 1)==-1) {
           perror("semop5");
           return EXIT_FAILURE;
       }
	
       printf("Antwort vom Server: %s\n", shm);
	
       /* exclusiven Zugriff auf Server freigeben V(0)*/
       semops.sem_num=0;
       semops.sem_op=1;
       semops.sem_flg=0;
       if (semop(semid, &semops, 1)==-1) {
             perror("semop6");
             return EXIT_FAILURE;
       } 
   }
   return EXIT_SUCCESS;
}

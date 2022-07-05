#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define SHMSZ 256
#define NSEM 3
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
       /* union semun is defined by including <sys/sem.h> */
#else
       /* according to X/OPEN we have to define it ourselves */
union semun {
        int val;                    /* value for SETVAL */
        struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */              
	unsigned short int *array;  /* array for GETALL, SETALL */
        struct seminfo *__buf;      /* buffer for IPC_INFO */
};
#endif                                      

int main (int argc, char **argv)
/* Server mit Kommunikation über Shared Memory / Semaphore. 
   Arbeitsweise:
 -  Server legt SHM-Bereich und Semaphoren an.
    Server wartet an Semaphor 1, bis ein Client signalisiert, dass er
    im SHM-Bereich einen Auftrag hinterlegt hat und den Zugriff auf SHM freigibt.
 - Server hinterlegt Antwort. 
 - Server gibt Zugriff auf SHM durch Client frei  (Sem. 2).
    Mit '0' beginnende Nachricht vom Client beendet Server, 
   gibt Ressourcen zurück.
*/

{
    int semid, shmid, zaehler=0, done;
    key_t key;
    char *shm;
    unsigned short *array;
    struct sembuf semops;

    array = (unsigned short *)malloc(NSEM*sizeof(unsigned short));

    /* Key für shm und sem aus Servernamen ableiten */
    if((key = ftok(argv[0], '1')) < 0) {
    	perror("ftok");
    	return EXIT_FAILURE;
    }

    /* Shared memory anfordern, Besitzer und Gruppe dürfen lesen und 
       schreiben */  
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0660)) < 0) {
	perror("shmget");
	return EXIT_FAILURE;
    }	
    
    /* Semaphorgruppe aus NSEM (3) Semaphoren anfordern */
    if ((semid = semget(key, NSEM, IPC_CREAT | 0660)) < 0) {
	perror("semget");
	return EXIT_FAILURE;
    }
   
    /* Semaphoren initialisieren */
    array[0]=1;  /* ein Clientprozess kann gleichzeitig über SHM mit uns kommunizieren */
    array[1]=0;  /* Server hat noch keinen Auftrag erhalten */
    array[2]=0;  /* Client hat noch keine Antwort erhalten */ 
    
    if (semctl(semid, 0, SETALL, (union semun) array)==-1) {
        perror("seminit");
	return EXIT_FAILURE;
    }
   
    /* Shared Memory in Adressraum einblenden */
    if ((shm = shmat(shmid, NULL, 0)) == (char *)(-1)) {
	perror("shmat");
	return EXIT_FAILURE;
    }

    
    do { 
	zaehler++;
        /* auf Semaphor 1 warten P(1): signalisiert Anfrage 
	 durch den Client */
        semops.sem_num=1;
        semops.sem_op=-1;
        semops.sem_flg=0; 
	
        if (semop(semid, &semops, 1)==-1) {
           perror("semop1");
           return EXIT_FAILURE;
        }
	
	done = (shm[0]=='0');
	printf(" Vom Client: %s\n", shm);
	sprintf(shm, "Vielen Dank, %d-te Nachricht erhalten\n", zaehler);
	sleep(5);
	
        /* Antwort signalisieren (Semaphor 2) V(2)*/
	semops.sem_num=2;
	semops.sem_op=1;
	semops.sem_flg=0;
	
	if (semop(semid, &semops, 1)==-1) {
           perror("semop3");
           return EXIT_FAILURE;
        }
    } while (!done);

    /* SHM-Bereich freigeben */
    if (shmctl(shmid, IPC_RMID, NULL)==-1) {
	perror("shmctl");
	return EXIT_FAILURE;
    }
    /* Semaphore freigeben */
    if (semctl(semid, 0, IPC_RMID, NULL)==-1) {
	perror("semctl");
	return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

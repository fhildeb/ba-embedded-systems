#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include "simplesema.h"

/* Kopie aus Manual semctl(2) */ 
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
       /* union semun is defined by including <sys/sem.h> */
#else
       /* according to X/OPEN we have to define it ourselves */
       union semun {
               int val;                 	/* value for SETVAL */
               struct semid_ds *buf; 		/* buffer for IPC_STAT, IPC_SET */
               unsigned short int *array; 	/* array for GETALL, SETALL */
               struct seminfo *__buf;   	/* buffer for IPC_INFO */
       };
#endif
                                                                          
/* Semaphor anlegen und auf value initialisieren  */ 
int initSema(key_t key, int value)
{
	int semid;
	union semun args;
	
	semid = semget(key, 1, IPC_CREAT|IPC_EXCL|0600);
	
	if(semid >=0) {			/* wenn korrekt angelegt */
		args.val=value;		/* mit value initialisieren */
		if(semctl(semid, 0, SETVAL, args) == -1) {
			perror("semctl");
			exit(1);
		}
		return semid;	
	} 

	if (errno == EEXIST)  {	   /* wenn Semaphor bereits existiert */	
		semid = semget(key, 1, 0);	/* öffnen */
		return semid;
	} else  {
		return (-1);
	}	
}
		
int P(int semid, int val)  /* Semaphor anfordern */
{
	struct sembuf buf;
	buf.sem_num=0;
	buf.sem_op=-val;
	buf.sem_flg=0;
	return semop(semid, &buf, 1);
}

int V(int semid, int val)  /* Semaphor freigeben */
{
	struct sembuf buf;
	buf.sem_num=0;
	buf.sem_op=val;
	buf.sem_flg=0;
	return semop(semid, &buf, 1);
}


